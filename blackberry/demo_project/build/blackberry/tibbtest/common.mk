ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=

# Extra include path for libfreetype and for target overrides and patches
EXTRA_INCVPATH+=$(QNX_TARGET)/usr/include/freetype2 \
	$(QNX_TARGET)/../target-override/usr/include \
	$(PROJECT_ROOT)/../../../../libv8/include \
	$(PROJECT_ROOT)/../../../../tibb/include
	
# Extra library search path for target overrides and patches
EXTRA_LIBVPATH+=$(QNX_TARGET)/$(CPUVARDIR)/usr/lib \
	$(QNX_TARGET)/$(CPUVARDIR)/usr/lib/qt4/lib \
	$(PROJECT_ROOT)/../../../../libv8/lib/$(CPU) \
	$(PROJECT_ROOT)/../../../../tibb/$(CPU)/a$(if $(filter arm,$(CPULIST)),.le-v7,)$(if $(filter g,$(VARIANTS)),-g,)

# Compiler options for enhanced security and recording the compiler options in release builds
CCFLAGS+=-fstack-protector-all -D_FORTIFY_SOURCE=2 \
	$(if $(filter g so shared,$(VARIANTS)),,-fPIE) \
	$(if $(filter g,$(VARIANTS)),,-frecord-gcc-switches)

# Linker options for enhanced security
LDFLAGS+=-Wl,-z,relro -Wl,-z,now $(if $(filter g so shared,$(VARIANTS)),,-pie)

# Add your required library names, here
LIBS+=tibb QtCascades_main v8 v8preparser

# Extra .so required
LDFLAGS+=-lQtCascades -lQtCore -lQtDeclarative -lQtGui -lQtMultimedia -lQtNetwork -lm -lbb
LDFLAGS+=-lQtOpenGL -lQtScript -lQtScriptTools -lQtSql -lQtSvg -lQtTest -lQtXml -lheimdall 

include $(MKFILES_ROOT)/qmacros.mk

# Suppress the _g suffix from the debug variant
BUILDNAME=$(IMAGE_PREF_$(BUILD_TYPE))$(NAME)$(IMAGE_SUFF_$(BUILD_TYPE))

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
