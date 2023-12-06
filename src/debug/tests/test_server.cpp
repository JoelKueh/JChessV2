
#include "../../common/CB_2_0/BoardRep.h"
#include "../../cppsrv/utils/server.h"
#include "../../cppsrv/utils/net_utils.h"

int main()
{
	server srv;
	while (srv.poll_set());
	std::cout << "Exiting..." << std::endl;
}
