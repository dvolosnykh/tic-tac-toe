function commonCxxFlags(qbs)
{
    var flags = [];
    if (qbs.toolchain.contains("clang"))
        flags.push("-std=c++11");
    else if (qbs.toolchain.contains("gcc"))
        flags.push("-std=c++0x");
    return flags;
}

function commonLinkerFlags(qbs)
{
    var flags = [];
    if (qbs.toolchain.contains("clang"))
        flags.push("-stdlib=libc++");
    return flags;
}
