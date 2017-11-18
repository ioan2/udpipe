#include "timestamp.h"

#include <time.h>
#include <cstring>

using std::string;

string timestamp(time_t start, unsigned currentepoch, unsigned epochs) {
    time_t t = time(0);
    struct tm * now = localtime(&t);
    char now_s[200];
    //strftime(now_s, 100, "%F %T %z", now);
    strftime(now_s, 100, "%F %T", now);

    time_t elapsed = t - start;

    char eta_s[50] = "";
    //std::cerr << currentepoch << " " << epochs << std::endl;
    if (currentepoch != 0 && epochs != 0) {
        // calculate ETA
        time_t eta = (elapsed * epochs / currentepoch) - elapsed;
        if (eta >= 1) {
            if (eta < 240)
                sprintf(eta_s, " ETA: %lds", eta);
            else if (eta < 3600)
                sprintf(eta_s, " ETA: %ldm", eta / 60);
            else
                sprintf(eta_s, " ETA: %.1fh", eta / 3600.0);
        }
    }

    char *ptr = now_s + strlen(now_s);
    if (elapsed < 240)
        sprintf(ptr, " [%lds]%s", elapsed, eta_s);
    else if (elapsed < 3600)
        sprintf(ptr, " [%ldm]%s", elapsed / 60, eta_s);
    else
        sprintf(ptr, " [%.1fh]%s", elapsed / 3600.0, eta_s);
    return string(now_s);
}
