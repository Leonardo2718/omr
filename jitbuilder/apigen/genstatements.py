import genutils

if __name__ == "__main__":
    api_file = "jitbuilder.api.json"
    dest_file = "StatementNames.hpp"

    with open(api_file) as api_src:
        api_description = genutils.APIDescription.load_json_file(api_src)

    with open(dest_file, "w") as writer:
        writer.write("#ifndef OMR_STATEMENTNAMES_INCL\n")
        writer.write("#define OMR_STATEMENTNAMES_INCL\n")

        writer.write("#include <cstdint>\n")

        writer.write("namespace OMR {\n")
        writer.write("namespace StatementNames {\n")

        version = api_description.description["version"] # need to add methods for this to APIDescription
        writer.write("static const int16_t VERSION_MAJOR = {};\n".format(version["major"]))
        writer.write("static const int16_t VERSION_MINOR = {};\n".format(version["minor"]))
        writer.write("static const int16_t VERSION_PATCH = {};\n".format(version["patch"]))

        writer.write('static const char * const RECORDER_SIGNATURE = "JBIL";\n')
        writer.write('static const char * const JBIL_COMPLETE      = "Done";\n')

        for service in api_description.get_class_by_name("MethodBuilder").services():
            name = service.name()
            writer.write('static const char * const STATEMENT_{} = "{}";\n'.format(name.upper(), name))
        for service in api_description.get_class_by_name("IlBuilder").services():
            name = service.name()
            writer.write('static const char * const STATEMENT_{} = "{}";\n'.format(name.upper(), name))

        writer.write('static const char * const STATEMENT_ID16BIT = "ID16BIT";\n')
        writer.write('static const char * const STATEMENT_ID32BIT = "ID32BIT";\n')

        writer.write("} // namespace StatementNames\n")
        writer.write("} // namespace OMR\n")

        writer.write("#endif // OMR_STATEMENTNAMES_INCL\n")
