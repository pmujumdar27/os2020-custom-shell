struct parsed_cmd{
    int argc;
    char **argv;
    int bgex;
};
struct parsed_cmd *create_cmd(int argc, char**argv);