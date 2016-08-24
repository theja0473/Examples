#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/watchdog.h>

int main(void)
{
    int32_t slFileDescT = open("/dev/watchdog", O_WRONLY);
    int32_t slTimeoutT = 45;  // a time in seconds

    // Send time request to the driver.
    ioctl(slFileDescT, WDIOC_SETTIMEOUT, &slTimeoutT);
    printf("The timeout was set to %d seconds\n", slTimeoutT);

    if (slFileDescT == -1)
    {
      perror("watchdog");
      exit(EXIT_FAILURE);
    }
    close(slFileDescT);

    return 0;
}
