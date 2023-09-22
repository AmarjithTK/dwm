/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* -------------------------- Endeavour dwm 2 ------------------------------- */

static const unsigned int borderpx = 3;          /* border pixel of windows */
static const unsigned int gappx = 9;              /* gaps between windows */
static const unsigned int snap = 32;              /* snap pixel */
static const unsigned int systraypinning = 0;     /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;      /* 0: systray in the right corner, >0: systray on the left of status text */
static const unsigned int systrayspacing = 4;     /* systray spacing */
static const int systraypinningfailfirst = 1;     /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor */
static const int showsystray = 1;                 /* 0 means no systray */
static const int showbar = 1;                     /* 0 means no bar */
static const int topbar = 1;                      /* 0 means bottom bar */
static const char *fonts[] = { "Roboto:size=13" }; /* Use the Roboto 13 font */
static const char dmenufont[] = "Roboto:size=13"; /* Use the same font for dmenu */


static const char col_gray1[] = "#2e3440"; /* Nord background color */
static const char col_gray2[] = "#3b4252"; /* Nord window border color */
static const char col_gray3[] = "#eceff4"; /* Nord foreground color */
static const char col_gray4[] = "#d8dee9"; /* Nord selected window border color */
static const char col_cyan[] = "#88c0d0";  /* Nord accent color */
static const char col_window[] = "#4c566a"; 


static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel] = { col_gray4, col_cyan, col_window },
};

static const char *const autostart[] = {
    "sh", "-c", "~/dwm/autostart.sh", NULL,
    NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
    /* xprop(1):
     * WM_CLASS(STRING) = instance, class
     * WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */

    { "kdeconnect.app", NULL, NULL, 0, 1, -1, 50, 50, 800, 500, 5 },
};

/* layout(s) */
static const float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;     /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=", tile },     /* first entry is the default */
    { "><>", NULL },     /* no layout function means floating behavior */
    { "[M]", monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask

#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/*------------------------------ commands ---------------------------*/
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[] = { "kitty", NULL };

static const char *volupcmd[] = { "/bin/bash", "-c", "amixer -D default sset Master Playback 5%+" };
static const char *volmaxcmd[] = { "/bin/bash", "-c", "amixer -D default sset Master Playback 100%" };
static const char *voldowncmd[] = { "/bin/bash", "-c", "amixer -D default sset Master Playback 5%-" };
static const char *volmutecmd[] = { "/bin/bash", "-c", "amixer -D default sset Master Playback 0%" };

static const Key keys[] = {
    // Basic keys
    /* modifier                     key        function        argument */
    { MODKEY, XK_p, spawn, {.v = dmenucmd } },
    { MODKEY|ShiftMask, XK_Return, spawn, {.v = termcmd } },
    { MODKEY, XK_b, togglebar, {0} },
    { MODKEY, XK_j, focusstack, {.i = +1 } },
    { MODKEY, XK_k, focusstack, {.i = -1 } },
    { MODKEY, XK_i, incnmaster, {.i = +1 } },
    { MODKEY, XK_d, incnmaster, {.i = -1 } },
    { MODKEY, XK_h, setmfact, {.f = -0.05} },
    { MODKEY, XK_l, setmfact, {.f = +0.05} },
    { MODKEY, XK_Return, zoom, {0} },
    { MODKEY, XK_Tab, view, {0} },
    { MODKEY|ShiftMask, XK_x, killclient, {0} },
    { MODKEY, XK_t, setlayout, {.v = &layouts[0]} },
    { MODKEY, XK_f, setlayout, {.v = &layouts[1]} },
    { MODKEY, XK_m, setlayout, {.v = &layouts[2]} },
    { MODKEY, XK_space, setlayout, {0} },
    { MODKEY|ShiftMask, XK_space, togglefloating, {0} },
    { MODKEY, XK_0, view, {.ui = ~0 } },
    { MODKEY|ShiftMask, XK_0, tag, {.ui = ~0 } },
    { MODKEY, XK_comma, focusmon, {.i = -1 } },
    { MODKEY, XK_period, focusmon, {.i = +1 } },
    { MODKEY|ShiftMask, XK_comma, tagmon, {.i = -1 } },
    { MODKEY|ShiftMask, XK_period, tagmon, {.i = +1 } },
    { MODKEY, XK_minus, setgaps, {.i = -1 } },
    { MODKEY, XK_equal, setgaps, {.i = +1 } },
    { MODKEY|ShiftMask, XK_equal, setgaps, {.i = 0 } },

    // Scratchpad keybinds
    { MODKEY, XK_s, scratchpad_show, {.i = 1} },
    { MODKEY, XK_y, scratchpad_show, {.i = 2} },
    { MODKEY, XK_u, scratchpad_show, {.i = 3} },
    { MODKEY|ShiftMask, XK_s, scratchpad_hide, {.i = 1} },
    { MODKEY|ShiftMask, XK_y, scratchpad_hide, {.i = 2} },
    { MODKEY|ShiftMask, XK_u, scratchpad_hide, {.i = 3} },
    { MODKEY|ShiftMask, XK_r, scratchpad_remove, {0} },

    // Volume control keybinds
    { 0, XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd} },
    { 0, XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd} },
    { MODKEY, XK_Up, spawn, {.v = volmaxcmd} },
    { 0, XF86XK_AudioMute, spawn, {.v = volmutecmd} },

    // Tag keys
    TAGKEYS(XK_1, 0)
    TAGKEYS(XK_2, 1)
    TAGKEYS(XK_3, 2)
    TAGKEYS(XK_4, 3)
    TAGKEYS(XK_5, 4)
    TAGKEYS(XK_6, 5)
    TAGKEYS(XK_7, 6)
    TAGKEYS(XK_8, 7)
    TAGKEYS(XK_9, 8)
    { MODKEY|ShiftMask, XK_q, quit, {0} },
    { MODKEY|ControlMask|ShiftMask, XK_q, quit, {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkTagBar, MODKEY, Button1, tag, {0} },
    { ClkTagBar, MODKEY, Button3, toggletag, {0} },
    { ClkWinTitle, 0, Button2, zoom, {0} },
    { ClkStatusText, 0, Button2, spawn, {.v = termcmd } },
    { ClkClientWin, MODKEY, Button1, movemouse, {0} },
    { ClkClientWin, MODKEY, Button2, togglefloating, {0} },
    { ClkClientWin, MODKEY, Button3, resizemouse, {0} },
    { ClkTagBar, 0, Button1, view, {0} },
    { ClkTagBar, 0, Button3, toggleview, {0} },
    { ClkTagBar, MODKEY, Button1, tag, {0} },
    { ClkTagBar, MODKEY, Button3, toggletag, {0} },
};

