struct parsed_cmd{
    int argc;
    char **argv;
};
struct parsed_cmd *create_cmd(int argc, char**argv);