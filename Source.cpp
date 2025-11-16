#include <stdlib.h>
#include <stdio.h>
#include "Application.h"

int main(void)
{
    Application app(800, 600, "KUZ_0061");

    if (!app.initialize())
    {
        fprintf(stderr, "Failed to initialize application\n");
        return EXIT_FAILURE;
    }

    app.run();

    return EXIT_SUCCESS;
}