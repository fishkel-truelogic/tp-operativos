#ifndef CPU_H_
#define CPU_H_

#include <src/fereStream.h>
#include <src/fereTcb.h>

Tcb* getCurrentTcb();
StrCpuKer* getSCK();
StrKerCpu* getSKC();
SocketClient* getMspClient();


#endif /* CPU_H_ */
