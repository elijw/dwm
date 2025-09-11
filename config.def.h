/* See LICENSE file for copyright and license details. */

// my cross-machine hack stuff
#define DESKTOP 0
#define LAPTOP  1

/*
 * wrap machine-conditional code with:
 *
 *     #if MACHINE == LAPTOP
 *       // laptop code
 *     #else
 *       // desktop code
 *     #endif
 *
 * or something similar
 *
 */

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 10;    /* snap pixel */
static const unsigned int gappih = 10;  /* horiz inner gap between windows */
static const unsigned int gappiv = 10;  /* vert inner gap between windows */
static const unsigned int gappoh =
    10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    10; /* vert outer gap between windows and screen edge */
static int smartgaps =
    0; /* 1 means no outer gap when there is only one window */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */

#if MACHINE == LAPTOP
static const int refreshrate = 60;
#else
static const int refreshrate = 144;
#endif

static const char* fonts[] = {"JetBrainsMono Nerd Font", "monospace:size=12"};
static const char dmenufont[] = "monospace:size=10";
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#005577";
static char selbgcolor[] = "#005577";

// PATCH_alpha
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;

static const unsigned int alphas[][3] = {
    /*               fg      bg        border*/
    [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeSel] = {OPAQUE, baralpha, borderalpha},
};

static const char* colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
    [SchemeSel] = {selfgcolor,  selbgcolor,  selbordercolor },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp",    NULL, NULL, 0,      1, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
typedef enum
{
    LAYOUT_TILE,
    LAYOUT_MONOCLE,
    LAYOUT_SPIRAL,
    LAYOUT_DWINDLE,
    LAYOUT_DECK,
    LAYOUT_BSTACK,
    LAYOUT_BSTACK_HORIZ,
    LAYOUT_GRID,
    LAYOUT_NROWGRID,
    LAYOUT_HORIZGRID,
    LAYOUT_GAPLESSGRID,
    LAYOUT_CENTERED_MASTER,
    LAYOUT_CENTERED_FLOATING_MASTER,
    LAYOUT_FLOATING,
    LAYOUT_LAST // sentinel for number of layouts
} LayoutId;

static const Layout layouts[] = {
    [LAYOUT_TILE] = {"[]=",  tile                  },
    [LAYOUT_MONOCLE] = {"[M]",  monocle               },
    [LAYOUT_SPIRAL] = {"[@]",  spiral                },
    [LAYOUT_DWINDLE] = {"[\\]", dwindle               },
    [LAYOUT_DECK] = {"H[]",  deck                  },
    [LAYOUT_BSTACK] = {"TTT",  bstack                },
    [LAYOUT_BSTACK_HORIZ] = {"===",  bstackhoriz           },
    [LAYOUT_GRID] = {"HHH",  grid                  },
    [LAYOUT_NROWGRID] = {"###",  nrowgrid              },
    [LAYOUT_HORIZGRID] = {"---",  horizgrid             },
    [LAYOUT_GAPLESSGRID] = {":::",  gaplessgrid           },
    [LAYOUT_CENTERED_MASTER] = {"|M|",  centeredmaster        },
    [LAYOUT_CENTERED_FLOATING_MASTER] = {">M>",  centeredfloatingmaster},
    [LAYOUT_FLOATING] = {"><>",  NULL                  },
};

/* key definitions */
#define ALT   Mod1Mask
#define SUPER Mod4Mask

#define TAGKEYS(KEY, TAG)                                                      \
    {SUPER, KEY, view, {.ui = 1 << TAG}},                                      \
        {SUPER | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},              \
        {SUPER | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                       \
        {SUPER | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
    {                                                                          \
        .v = (const char*[])                                                   \
        {                                                                      \
            "/bin/sh", "-c", cmd, NULL                                         \
        }                                                                      \
    }

/* commands */
#include "movestack.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */
    //{ SUPER,                       XK_r,      spawn,          {.v =
    // dmenucmd } },
    {SUPER | ControlMask | ShiftMask, XK_1,      spawn,          {.v = flameshotcmd}             },
    {SUPER,                           XK_r,      spawn,          {.v = dmenucmd}                 },
    {SUPER | ShiftMask,               XK_r,      spawn,          {.v = roficmd}                  },
    {SUPER,                           XK_Return, spawn,          {.v = kittycmd}                 },
    {SUPER,                           XK_b,      togglebar,      {0}                             },
    {SUPER,                           XK_j,      focusstack,     {.i = +1}                       },
    {SUPER,                           XK_k,      focusstack,     {.i = -1}                       },
    {SUPER,                           XK_i,      incnmaster,     {.i = +1}                       },
    {SUPER,                           XK_d,      incnmaster,     {.i = -1}                       },
    {SUPER,                           XK_h,      setmfact,       {.f = -0.05}                    },
    {SUPER,                           XK_l,      setmfact,       {.f = +0.05}                    },
    {SUPER | ShiftMask,               XK_j,      movestack,      {.i = +1}                       },
    {SUPER | ShiftMask,               XK_k,      movestack,      {.i = -1}                       },
    {SUPER,                           XK_z,      zoom,           {0}                             },
	//{SUPER | Mod1Mask,                XK_u,      incrgaps,       {.i = +1} },
    //{SUPER | Mod1Mask | ShiftMask,    XK_u,      incrgaps,       {.i = -1} },
    //{SUPER | Mod1Mask,                XK_i,      incrigaps,      {.i = +1} },
	//{SUPER | Mod1Mask | ShiftMask,    XK_i,      incrigaps,      {.i = -1} },
    //{SUPER | Mod1Mask,                XK_o,      incrogaps,      {.i = +1} },
	//{SUPER | Mod1Mask | ShiftMask,    XK_o,      incrogaps,      {.i = -1} },
    //{SUPER | Mod1Mask,                XK_6,      incrihgaps,     {.i = +1} },
	//{SUPER | Mod1Mask | ShiftMask,    XK_6,      incrihgaps,     {.i = -1} },
    //{SUPER | Mod1Mask,                XK_7,      incrivgaps,     {.i = +1} },
	//{SUPER | Mod1Mask | ShiftMask,    XK_7,      incrivgaps,     {.i = -1} },
    //{SUPER | Mod1Mask,                XK_8,      incrohgaps,     {.i = +1} },
	//{SUPER | Mod1Mask | ShiftMask,    XK_8,      incrohgaps,     {.i = -1} },
    //{SUPER | Mod1Mask,                XK_9,      incrovgaps,     {.i = +1} },
    //{SUPER | Mod1Mask | ShiftMask,    XK_9,      incrovgaps,     {.i = -1} },
	{SUPER | ALT,                     XK_0,      togglegaps,     {0}                             },
	//{SUPER | Mod1Mask | ShiftMask,    XK_0,      defaultgaps,    {0} },
    {SUPER,                           XK_Tab,    view,           {0}                             },
    {SUPER,                           XK_q,      killclient,     {0}                             },
    {SUPER,                           XK_t,      setlayout,      {.v = &layouts[LAYOUT_TILE]}    },
    {SUPER,                           XK_f,      setlayout,      {.v = &layouts[LAYOUT_FLOATING]}},
    {SUPER,                           XK_m,      setlayout,      {.v = &layouts[LAYOUT_MONOCLE]} },
    {SUPER,                           XK_s,      setlayout,      {.v = &layouts[LAYOUT_BSTACK]}  },
    {SUPER,                           XK_space,  setlayout,      {0}                             },
    {SUPER | ShiftMask,               XK_space,  togglefloating, {0}                             },
    {SUPER,                           XK_0,      view,           {.ui = ~0}                      },
    {SUPER | ShiftMask,               XK_0,      tag,            {.ui = ~0}                      },
    {SUPER,                           XK_comma,  focusmon,       {.i = -1}                       },
    {SUPER,                           XK_period, focusmon,       {.i = +1}                       },
    {SUPER | ShiftMask,               XK_comma,  tagmon,         {.i = -1}                       },
    {SUPER | ShiftMask,               XK_period, tagmon,         {.i = +1}                       },
    {SUPER,                           XK_F5,     xrdb,           {.v = NULL}                     },
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){SUPER | ShiftMask,               XK_q,      quit,           {0}                             },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
/*
 *    dwmc signal wrappers
 */
void setlayoutex(const Arg* arg)
{
    setlayout(&((Arg){.v = &layouts[arg->i]}));
}

void viewex(const Arg* arg)
{
    view(&((Arg){.ui = 1 << arg->ui}));
}

void viewall(const Arg* arg)
{
    view(&((Arg){.ui = ~0}));
}

void toggleviewex(const Arg* arg)
{
    toggleview(&((Arg){.ui = 1 << arg->ui}));
}

void tagex(const Arg* arg)
{
    tag(&((Arg){.ui = 1 << arg->ui}));
}

void toggletagex(const Arg* arg)
{
    toggletag(&((Arg){.ui = 1 << arg->ui}));
}

void tagall(const Arg* arg)
{
    tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"`
 */
static Signal signals[] = {
    /* signum           function */
    {"focusstack",     focusstack    },
    {"setmfact",       setmfact      },
    {"togglebar",      togglebar     },
    {"incnmaster",     incnmaster    },
    {"togglefloating", togglefloating},
    {"focusmon",       focusmon      },
    {"tagmon",         tagmon        },
    {"zoom",           zoom          },
    {"view",           view          },
    {"viewall",        viewall       },
    {"viewex",         viewex        },
    {"toggleview",     view          },
    {"toggleviewex",   toggleviewex  },
    {"tag",            tag           },
    {"tagall",         tagall        },
    {"tagex",          tagex         },
    {"toggletag",      tag           },
    {"toggletagex",    toggletagex   },
    {"killclient",     killclient    },
    {"quit",           quit          },
    {"setlayout",      setlayout     },
    {"setlayoutex",    setlayoutex   },
    {"xrdb",           xrdb          },
};
