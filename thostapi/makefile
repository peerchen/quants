#ͨ��makefile
#���
#����׼��: make config
#����debug�棺make 
#����release�棺
# Ȼ��make -e DEBUG=0
#����release��Ѷ�汾
# make -e DEBUG=0 PRO=E86

include ../public/makefile.define

CPPFLAGS := $(CPPFLAGS) -I. -I$(APIDIR) -I$(CIFOXDIR) -I$(CIFOXDIR)/mysql -I$(CIFOXDIR)/ctp -I$(CIFOXDIR)/sgit                                                           
SRCDIRS := $(SRCDIRS)  $(APIDIR)/ctphandler $(APIDIR)/fshandler $(APIDIR)/kbarhandler  $(APIDIR)/simulationhandler $(APIDIR)/thostapihandler

INLIBS  := $(INLIBS) $(CIFOXLIBDIR)/libmysqlclient_r.a $(CIFOXLIBDIR)/libjsoncpp_r.a

LDFLAGS := $(LDFLAGS) -L$(CIFOXLIBDIR) -lthostmduserapi -lthosttraderapi -lsgitquotapi -lsgittradeapi -llog4cplus                                                               
PROGRAM := $(PROGRAMDIR)/lib/thostapi.so

include ../public/makefile.in
