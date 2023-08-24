#include <stdlib.h>
#include <string.h>

#include "v7.h"

typedef uint64_t val_t;

static void show_usage(char *argv[])
{
    fprintf(stderr, "V7 version %s (c) Cesanta Software, built on %s\n",
            V7_VERSION, __DATE__);
    fprintf(stderr, "Usage: %s [OPTIONS] ...\n", argv[0]);
    fprintf(stderr, "%s\n", "OPTIONS:");
    fprintf(stderr, "%s\n", "  -e <expr>            execute expression");
    fprintf(stderr, "%s\n", "  -mm                  dump memory stats");
    fprintf(stderr, "%s\n", "  -vo <n>              object arena size");
    fprintf(stderr, "%s\n", "  -vf <n>              function arena size");
    fprintf(stderr, "%s\n", "  -vp <n>              property arena size");
    exit(EXIT_FAILURE);
}

int v7_main(int argc, char *argv[], void (*pre_freeze_init)(struct v7 *),
            void (*pre_init)(struct v7 *), void (*post_init)(struct v7 *))
{
    int exit_rcode = EXIT_SUCCESS;
    struct v7 *v7;
    struct v7_create_opts opts;
    int as_json = 0;
    int i, j, dump_stats = 0;
    val_t res;
    int nexprs = 0;
    const char *exprs[16];

    memset(&opts, 0, sizeof(opts));

    /* Execute inline code */
    for (i = 1; i < argc && argv[i][0] == '-'; i++)
    {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc)
        {
            exprs[nexprs++] = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            show_usage(argv);
        }
        else if (strcmp(argv[i], "-j") == 0)
        {
            as_json = 1;
        }
        else if (strcmp(argv[i], "-vo") == 0 && i + 1 < argc)
        {
            opts.object_arena_size = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-vf") == 0 && i + 1 < argc)
        {
            opts.function_arena_size = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-vp") == 0 && i + 1 < argc)
        {
            opts.property_arena_size = atoi(argv[i + 1]);
            i++;
        }
    }

    v7 = v7_create_opt(opts);
    res = V7_UNDEFINED;

    if (pre_freeze_init != NULL)
    {
        pre_freeze_init(v7);
    }

    if (pre_init != NULL)
    {
        pre_init(v7);
    }

    (void)dump_stats;

    /* Execute files */
    //    for (; i < argc; i++)
    {
        if (v7_exec_file(v7, "ev7_runtime.js", &res) != V7_OK)
        //if (v7_exec_buf(v7, ev7_source, strlen(ev7_source), &res) != V7_OK)
        {
            v7_print_error(stderr, v7, argv[i], res);
            res = V7_UNDEFINED;
        }
    }

    /* Execute inline expressions */
    for (j = 0; j < nexprs; j++)
    {
        enum v7_err (*exec)(struct v7 *, const char *, v7_val_t *);
        exec = v7_exec;

        if (exec(v7, exprs[j], &res) != V7_OK)
        {
            v7_print_error(stderr, v7, exprs[j], res);
            exit_rcode = EXIT_FAILURE;
            res = V7_UNDEFINED;
        }
    }

    {
        char buf[2000];
        char *s = v7_stringify(v7, res, buf, sizeof(buf),
                               as_json ? V7_STRINGIFY_JSON : V7_STRINGIFY_DEBUG);
        printf("%s\n", s);
        if (s != buf)
        {
            free(s);
        }
    }

    if (post_init != NULL)
    {
        post_init(v7);
    }

    (void)dump_stats;

    v7_destroy(v7);
    return exit_rcode;
}

int main(int argc, char *argv[])
{
    return v7_main(argc, argv, NULL, NULL, NULL);
}
