#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_cc2640r2_sdk_4_20_00_04/source;C:/ti/simplelink_cc2640r2_sdk_4_20_00_04/kernel/tirtos/packages
override XDCROOT = C:/ti/ccs1000/xdctools_3_61_00_16_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_cc2640r2_sdk_4_20_00_04/source;C:/ti/simplelink_cc2640r2_sdk_4_20_00_04/kernel/tirtos/packages;C:/ti/ccs1000/xdctools_3_61_00_16_core/packages;..
HOSTOS = Windows
endif
