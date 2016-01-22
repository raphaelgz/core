ifeq ($(HB_BUILD_MODE),cpp)
   ifneq ($(findstring clang$(subst x, ,x)version$(subst x, ,x)1,$(shell clang --version)),)
      HB_BUILD_MODE := c
   endif
endif

ifeq ($(HB_BUILD_MODE),cpp)
   HB_CMP := clang++
else
   HB_CMP := clang
endif

OBJ_EXT := .o
LIB_PREF := lib
LIB_EXT := .a

CC := $(HB_CCACHE) $(HB_CCPREFIX)$(HB_CMP)$(HB_CCSUFFIX)
ifneq ($(filter --analyze, $(HB_USER_CFLAGS)),)
   CC_IN :=
else
   CC_IN := -c
endif
# NOTE: Works also without the ending space after -o.
CC_OUT := -o$(subst x,x, )

CFLAGS += -I. -I$(HB_HOST_INC)

# -fno-common enables building .dylib files
CFLAGS += -fno-common

ifneq ($(HB_BUILD_WARN),no)
   CFLAGS += -W -Wall
else
   CFLAGS += -Wmissing-braces -Wreturn-type -Wformat
   ifneq ($(HB_BUILD_MODE),cpp)
      CFLAGS += -Wimplicit-int -Wimplicit-function-declaration
   endif
endif

ifneq ($(HB_BUILD_OPTIM),no)
   CFLAGS += -O3
endif

ifeq ($(HB_BUILD_DEBUG),yes)
   CFLAGS += -g
endif

# It's to avoid warning message generated when 'long double' is used
# remove it if you have newer compiler version
#CFLAGS += -Wno-long-double

LD := $(CC)
LD_OUT := -o$(subst x,x, )

LIBPATHS := $(foreach dir,$(LIB_DIR) $(SYSLIBPATHS),-L$(dir))
LDLIBS := $(foreach lib,$(HB_USER_LIBS) $(LIBS) $(SYSLIBS),-l$(lib))

LDFLAGS += $(LIBPATHS)

AR := libtool
AR_RULE = ( $(AR) -static -no_warning_for_no_symbols $(ARFLAGS) $(HB_AFLAGS) $(HB_USER_AFLAGS) -o $(LIB_DIR)/$@ $(^F) $(ARSTRIP) ) || ( $(RM) $(LIB_DIR)/$@ && $(FALSE) )

DY := $(AR)
DFLAGS += -dynamic -flat_namespace -undefined warning -multiply_defined suppress -single_module $(LIBPATHS)
DY_OUT := -o$(subst x,x, )
DLIBS := $(foreach lib,$(HB_USER_LIBS) $(SYSLIBS),-l$(lib))

DY_RULE = $(DY) $(DFLAGS) -install_name "$(DYN_NAME_NVR)" -compatibility_version $(HB_VER_MAJOR).$(HB_VER_MINOR) -current_version $(HB_VER_MAJOR).$(HB_VER_MINOR).$(HB_VER_RELEASE) $(HB_USER_DFLAGS) $(DY_OUT)$(DYN_DIR)/$@ $^ $(DLIBS) $(DYSTRIP) && $(LN) $(@F) $(DYN_FILE_NVR) && $(LN) $(@F) $(DYN_FILE_CPT)

include $(TOP)$(ROOT)config/rules.mk
