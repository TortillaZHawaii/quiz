################################################################################
#
# quiz
#
################################################################################

QUIZ_VERSION = 1.0
QUIZ_SITE = $(TOPDIR)/package/quiz/src
QUIZ_SITE_METHOD = local
QUIZ_DEPENDENCIES = libgpiod

define QUIZ_BUILD_CMDS
   $(MAKE) $(TARGET_CONFIGURE_OPTS) quiz -C $(@D)
endef
define QUIZ_INSTALL_TARGET_CMDS 
   $(INSTALL) -D -m 0755 $(@D)/quiz $(TARGET_DIR)/usr/bin 
endef
QUIZ_LICENSE = Proprietary

$(eval $(generic-package))