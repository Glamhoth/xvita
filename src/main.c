#include <psp2/kernel/modulemgr.h>
#include <psp2/types.h>
#include <taihen.h>
#include "logger.h"

#define SN30PRO_VID 0x045E
#define SN30PRO_PID 0x02E0

static SceUID sceBt_sub_22999C8_hook_uid = -1;
static tai_hook_ref_t sceBt_sub_22999C8_ref;

void _start() __attribute__ ((weak, alias("module_start")));

static int
is_sn30pro(const unsigned short vid_pid[2])
{
    return vid_pid[0] == SN30PRO_VID && vid_pid[1] == SN30PRO_PID;
}

static int
sceBt_sub_22999C8_hook_func(void* dev_base_ptr, int r1)
{
    unsigned int flags = *(unsigned int*) (r1 + 4);

    if(dev_base_ptr && !(flags & 2))
    {
        const void* dev_info = *(const void**) (dev_base_ptr + 0x14A4);
        const unsigned short* vid_pid = (const unsigned short*) (dev_info + 0x28);

        if(is_sn30pro(vid_pid)) {
            LOG("8BitDo SN30pro connected\n");
        }
        logger_flush();
    }

    return TAI_CONTINUE(int, sceBt_sub_22999C8_ref, dev_base_ptr, r1);
}

int
module_start(SceSize argc, const void* args)
{
    logger_reset();

    LOG("xvita by glamhoth\n");

    tai_module_info_t sceBt_modinfo;
    sceBt_modinfo.size = sizeof(sceBt_modinfo);

    LOG("Looking for Bluetooth module\n");
    int ret = taiGetModuleInfoForKernel(KERNEL_PID, "SceBt", &sceBt_modinfo);
    if(ret < 0)
    {
        LOG("Unable to find SceBt module\n");
        goto error;
    }

    LOG("Found SceBt module\n");

    LOG("Hooking SceBt\n");
    sceBt_sub_22999C8_hook_uid = taiHookFunctionOffsetForKernel(KERNEL_PID,
            &sceBt_sub_22999C8_ref, sceBt_modinfo.modid, 0,
            0x22999C8 - 0x2280000, 1, sceBt_sub_22999C8_hook_func);

    return SCE_KERNEL_START_SUCCESS;

error:
    logger_flush();
    return SCE_KERNEL_START_FAILED;
}


int
module_stop(SceSize argc, const void* args)
{
    if (sceBt_sub_22999C8_hook_uid > 0)
    {
        taiHookReleaseForKernel(sceBt_sub_22999C8_hook_uid, sceBt_sub_22999C8_ref);
    }

    return SCE_KERNEL_STOP_SUCCESS;
}
