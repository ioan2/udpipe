#pragma once

#include <iostream>
#include <time.h>


namespace ufal {
namespace udpipe {
namespace utils {

using std::string;

/** produces a timestamp of the current date and time 
 * @param start if not 0, timestamp will display elapsed time
 * @param currentepoch if not 0 displays estimated time to arrival
 * @param epochs if not 0 displays estimated time to arrival
 */
string timestamp(time_t start = 0, unsigned currentepoch=0, unsigned epochs=0);

} // namespace utils
} // namespace udpipe
} // namespace ufal
