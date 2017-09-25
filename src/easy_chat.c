
// Includes
#include "global.h"
#include "songs.h"
#include "sound.h"
#include "rom4.h"
#include "task.h"
#include "main.h"
#include "window.h"
#include "palette.h"
#include "easy_chat.h"

// Static type declarations

#define EZCHAT_TASK_STATE        0
#define EZCHAT_TASK_UNK01        1
#define EZCHAT_TASK_UNK02        2
#define EZCHAT_TASK_MAINCALLBACK 4
#define EZCHAT_TASK_UNK06        6
#define EZCHAT_TASK_UNK07        7

// Static RAM declarations
static void sub_811A2C0(u8);
static void sub_811A278(void);
static bool8 sub_811A428(u8);
static void sub_811A2FC(u8);
/*static*/ u16 sub_811AAAC(void);
/*static*/ bool32 sub_811A88C(u16);
/*static*/ void sub_811C158(u16);
/*static*/ bool8 sub_811C170(void);
/*static*/ void sub_811A8A4(u16);
/*static*/ void sub_811A4D0(MainCallback);
bool8 sub_811F28C(void);
bool8 sub_811A95C(u8, u32, u8);
bool8 sub_811BF8C(void);
bool8 sub_811BFA4(void);
void sub_811C13C(void);
void sub_811AA90(void);
void sub_811F2B8(void);

// Static ROM declarations

// .rodata

// .text

void sub_811A20C(u8 a0, u32 a1, MainCallback callback, u8 a3)
{
    u8 taskId;

    ResetTasks();
    taskId = CreateTask(sub_811A2C0, 0);
    gTasks[taskId].data[EZCHAT_TASK_UNK01] = a0;
    gTasks[taskId].data[EZCHAT_TASK_UNK07] = a3;
    SetWordTaskArg(taskId, EZCHAT_TASK_UNK02, a1);
    SetWordTaskArg(taskId, EZCHAT_TASK_MAINCALLBACK, (u32)callback);
    SetMainCallback2(sub_811A278);
}

static void sub_811A278(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void sub_811A290(void)
{
    TransferPlttBuffer();
    LoadOam();
    ProcessSpriteCopyRequests();
}

static void sub_811A2A4(u8 taskId, TaskFunc taskFunc)
{
    gTasks[taskId].func = taskFunc;
    gTasks[taskId].data[EZCHAT_TASK_STATE] = 0;
}

static void sub_811A2C0(u8 taskId)
{
    if (!is_c1_link_related_active())
    {
        while (sub_811A428(taskId));
    }
    else
    {
        if (sub_811A428(taskId) == TRUE)
        {
            return;
        }
    }
    sub_811A2A4(taskId, sub_811A2FC);
}

static void sub_811A2FC(u8 taskId)
{
    u16 v0;
    s16 *data;

    data = gTasks[taskId].data;
    switch (data[EZCHAT_TASK_STATE])
    {
        case 0:
            SetVBlankCallback(sub_811A290);
            BlendPalettes(-1, 16, 0);
            BeginNormalPaletteFade(-1, -1, 16, 0, 0);
            data[EZCHAT_TASK_STATE] = 5;
            break;
        case 1:
            v0 = sub_811AAAC();
            if (sub_811A88C(v0))
            {
                BeginNormalPaletteFade(-1, -2, 0, 16, 0);
                data[EZCHAT_TASK_STATE] = 3;
                data[EZCHAT_TASK_UNK06] = v0;
            }
            else if (v0 == 0x18)
            {
                BeginNormalPaletteFade(-1, -1, 0, 16, 0);
                data[EZCHAT_TASK_STATE] = 4;
            }
            else if (v0 != 0)
            {
                PlaySE(SE_SELECT);
                sub_811C158(v0);
                data[EZCHAT_TASK_STATE] ++;
            }
            break;
        case 2:
            if (!sub_811C170())
            {
                data[EZCHAT_TASK_STATE] = 1;
            }
            break;
        case 3:
            if (!gPaletteFade.active)
            {
                sub_811A8A4(data[EZCHAT_TASK_UNK06]);
            }
            break;
        case 4:
            if (!gPaletteFade.active)
            {
                sub_811A4D0((MainCallback)GetWordTaskArg(taskId, EZCHAT_TASK_MAINCALLBACK));
            }
            break;
        case 5:
            if (!gPaletteFade.active)
            {
                data[EZCHAT_TASK_STATE] = 1;
            }
            break;
    }
}

static bool8 sub_811A428(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    switch (data[EZCHAT_TASK_STATE])
    {
        case 0:
            SetVBlankCallback(NULL);
            ResetSpriteData();
            FreeAllSpritePalettes();
            ResetPaletteFade();
            break;
        case 1:
            if (!sub_811F28C())
            {
                sub_811A4D0((MainCallback)GetWordTaskArg(taskId, EZCHAT_TASK_MAINCALLBACK));
            }
            break;
        case 2:
            if (!sub_811A95C(data[EZCHAT_TASK_UNK01], GetWordTaskArg(taskId, EZCHAT_TASK_UNK02), data[EZCHAT_TASK_UNK07]))
            {
                sub_811A4D0((MainCallback)GetWordTaskArg(taskId, EZCHAT_TASK_MAINCALLBACK));
            }
            break;
        case 3:
            if (!sub_811BF8C())
            {
                sub_811A4D0((MainCallback)GetWordTaskArg(taskId, EZCHAT_TASK_MAINCALLBACK));
            }
            break;
        case 4:
            if (sub_811BFA4())
            {
                return TRUE;
            }
            break;
        default:
            return FALSE;
    }
    data[EZCHAT_TASK_STATE] ++;
    return TRUE;
}

void sub_811A4D0(MainCallback callback)
{
    sub_811C13C();
    sub_811AA90();
    sub_811F2B8();
    FreeAllWindowBuffers();
    SetMainCallback2(callback);
}
