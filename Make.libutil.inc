#
# 共用实用库的依赖项
#
$(LIBUTIL):
	(cd $(ROOT)/lib && $(MAKE))
