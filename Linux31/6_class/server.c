
#include "comm.h"

int main()
{
	char buf[256];
	int msgid = createMsgQueue();
	while(1){
		//recv
		//recvMsg(msgid, CLIENT_TYPE, buf);
		//if(strcmp(buf, "quit") == 0){
		//	printf("client is quit, me too!\n");
		//	break;
		//}
		printf("client# %s\n", buf);
		//send
		printf("Please Enter: ");
		scanf("%s", buf);
		sendMsg(msgid, buf, SERVER_TYPE);
	}
	destroyMsgQueue(msgid);
	return 0;
}
