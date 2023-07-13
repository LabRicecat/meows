#include "catpkgs/carescript-api/carescript-api.hpp"
#include "catpkgs/mathenyatica/mathenyatica.hpp"
#include "catpkgs/stack-nyachine/stack-nyachine.hpp"
#include "catpkgs/fnyactional/fnyactional.hpp"
#include "catpkgs/nyasm/nyasm.hpp"

#include "catpkgs/argparser/argparser.h"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

void help_message() {
    std::cout
    << "## meows\n"
    << "The LabRicecat language collection\n\n"
    << "Usage: meows <language> <files..>\n\n"
    << "language ::= \n"
    << "    -ccs -carescript       :  carescript\n"
    << "    -mnya -mathenyatica    :  mathenyatica\n"
    << "    -fnya -fnyactional     :  fnyactional\n"
    << "    -nyasm                 :  nyasm\n"
    << "    -snya -stack-nyachine  :  stack-nyachine\n\n"
    << "<files..> will get merged and fed to the interpreter or compiler\n"
    << "Repository: https://github.com/labricecat/meows\n";
}

std::string read_file(std::string file) {
    std::fstream f(file);
    std::string r;
    while(f.good()) r += f.get();
    if(r != "") r.pop_back();
    return r;
}

std::string merge_files(std::vector<std::string> files) {
    std::string r;
    for(auto& i : files) {
        if(!std::filesystem::exists(i)) {
            std::cout << "meows: no such file \"" + i + "\"\n";
            std::exit(2);
        }
        r += read_file(i) + "\n";
    }
    return r;
}

int main(int argc, char** argv) {
    ArgParser parser = ArgParser()
        .addArg("-ccs",ARG_TAG,{"-carescript"},0)
        .addArg("-mnya",ARG_TAG,{"-mathenyatica"},0)
        .addArg("-nyasm",ARG_TAG,{},0)
        .addArg("-fnya",ARG_TAG,{"-fnyactional"},0)
        .addArg("-snya",ARG_TAG,{"-stack-nyachine"},0)
        .setbin()
    ;

    ParsedArgs pargs = parser.parse(argv,argc);
    if(!pargs || !pargs.has_bin()) {
        help_message();
        return 1;
    }

    auto source = merge_files(pargs.get_bin());

    if(pargs["-ccs"]) {
        carescript::Interpreter interp;
        interp.pre_process(source).on_error([&](carescript::Interpreter&){
            std::cout << "meows: carescript: " << interp.error() << "\n";
        }).otherwise([&](carescript::Interpreter&){
            interp.run("main").on_error([&](carescript::Interpreter&){
                std::cout << "meows: carescript: " << interp.error() << "\n";
            });
        });
    }
    else if(pargs["-mnya"]) {
        mny_interpreter interp;
        interp.parse(source);
        std::string err;
        if((err = mny_error()) != "") {
            std::cout << "meows: mathenyatica: " << err << "\n";
            return 3;
        }
        auto res = interp.run_function("main");
        if((err = mny_error()) != "") {
            std::cout << "meows: mathenyatica: " << err << "\n";
            return 3;
        }
        std::cout << (res ? "TRUE":"FALSE") << "\n";
    }
    else if(pargs["-fnya"]) {
        fnyactional::evaluate(source);
        std::cout << fnyactional::call("main").stringify() << "\n";
    }
    else if(pargs["-nyasm"]) {
        try {
            auto machine = compile(source);
            stack_nyachine::tofile("out.snya",&machine);
            std::cout << "meows: compiled into out.snya\n";
        }
        catch(SyntaxException& err) {
            std::cout << "meows: nyasm: " << err.message << "\n";
        }
    }
    else if(pargs["-snya"]) {
        if(pargs.get_bin().size() != 1) {
            std::cout << "meows: stack-nachine: only one file allowed\n";
        }
        stack_nyachine::StackNyachine machine(0,0);
        stack_nyachine::fromfile(pargs.get_bin().front().c_str(),&machine);
        stack_nyachine::NyaSignal signal = stack_nyachine::ruwun(&machine,0);
        if(signal != stack_nyachine::NyaSignal::NYASIG_OK) {
            std::cout << "meows: nyasm: signal: " << signal << "\n";
            return 3;
        }
    }
    else {
        help_message();
        return 1;
    }
}