#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include "tomlc99/toml.h"
#include "toml11/toml.hpp"

static void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
    // exit(1);
}

void test_sample()
{
    FILE* fp;
    char errbuf[200];

    // 1. Read and parse toml file
    fp = fopen("sample.toml", "r");
    if (!fp) {
        error("cannot open sample.toml - ", strerror(errno));
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        error("cannot parse - ", errbuf);
    }

    // 2. Traverse to a table.
    toml_table_t* server = toml_table_in(conf, "server");
    if (!server) {
        error("missing [server]", "");
    }

    // 3. Extract values
    toml_datum_t host = toml_string_in(server, "host");
    if (!host.ok) {
        error("cannot read server.host", "");
    }

    toml_array_t* portarray = toml_array_in(server, "port");
    if (!portarray) {
        error("cannot read server.port", "");
    }

    printf("host: %s\n", host.u.s);
    printf("port: ");
    for (int i = 0; ; i++) {
        toml_datum_t port = toml_int_at(portarray, i);
        if (!port.ok) break;
        printf("%d ", (int)port.u.i);
    }
    printf("\n");

    // 4. Free memory
    free(host.u.s);
    toml_free(conf);
}

void test_sample2()
{
    FILE* fp;
    char errbuf[200];

    // 1. Read and parse toml file
    fp = fopen("sample2.toml", "r");
    if (!fp) {
        error("cannot open sample2.toml - ", strerror(errno));
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        error("cannot parse - ", errbuf);
    }

    // 2. Traverse to a table.
    toml_table_t* setting = toml_table_in(conf, "setting");
    if (!setting) {
        error("missing [setting]", "");
    }

    // 3. Extract values
    toml_datum_t enable_1 = toml_bool_in(setting, "enable_1");
    if (!enable_1.ok) {
        error("cannot read setting.enable_1", "");
    }

    printf("enable_1=%d\n", enable_1.u.b);

    toml_datum_t enable_2 = toml_bool_in(setting, "enable_2");
    if (!enable_2.ok) {
        error("cannot read setting.enable_2", "");
    }

    printf("enable_2=%d\n", enable_2.u.b);


    // 4. Free memory
    toml_free(conf);
}

void test_sample3()
{
    FILE* fp;
    char errbuf[200];

    // 1. Read and parse toml file
    fp = fopen("sample3.toml", "r");
    if (!fp) {
        error("cannot open sample2.toml - ", strerror(errno));
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        error("cannot parse - ", errbuf);
    }

    // 3. Extract values
    toml_datum_t enable_1 = toml_bool_in(conf, "enable_1");
    if (!enable_1.ok) {
        error("cannot read conf.enable_1", "");
    }

    printf("enable_1=%d\n", enable_1.u.b);

    toml_datum_t enable_2 = toml_bool_in(conf, "enable_2");
    if (!enable_2.ok) {
        error("cannot read conf.enable_2", "");
    }

    printf("enable_2=%d\n", enable_2.u.b);


    // 4. Free memory
    toml_free(conf);
}

void test_sample4()
{
    // ```toml
    // title = "an example toml file"
    // nums  = [3, 1, 4, 1, 5]
    // ```
    auto data = toml::parse("sample4.toml");

    // find a value with the specified type from a table
    std::string title = toml::find<std::string>(data, "title");
    std::cout << title << "\n";
    // convert the whole array into any container automatically
    std::vector<int> nums = toml::find<std::vector<int>>(data, "nums");

    bool enable_1 = toml::find<bool>(data, "enable_1");
    bool enable_2 = toml::find<bool>(data, "enable_2");
    std::cout << "enable_1:" << enable_1 << "\n";
    std::cout << "enable_2:" << enable_2 <<"\n";

    // access with STL-like manner
    if (!data.contains("foo"))
    {
        data["foo"] = "bar";
    }

    // pass a fallback
    std::string name = toml::find_or<std::string>(data, "name", "not found");
    std::cout << name << "\n";

    // width-dependent formatting
    std::cout << std::setw(80) << data << std::endl;
}

void test_write1()
{
    {
        const toml::value v{ {"a", 42} };
        const std::string fmt = toml::format(v);
        std::cout << "test_write1:\t";
        std::cout << fmt << "\n";
    }
   
    {
        const toml::value v{ {"a", "b"} };
        const std::string fmt = toml::format(v);
        std::cout << "test_write1:\t";
        std::cout << fmt << "\n";
    }

}

int main()
{
    test_sample();
    test_sample2();
    test_sample3();
    test_sample4();
    test_write1();
    system("pause");
    return 0;
}

