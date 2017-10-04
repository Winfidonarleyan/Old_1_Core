#include "GitRevision.h"
#include "revision.h"

char const* GitRevision::GetDate()
{
     return __DATE__;
}

char const* GitRevision::build_time()
{
     return __TIME__;
}