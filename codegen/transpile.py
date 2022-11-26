import json, os, sys, zipfile
from typing import Dict

def cxx_sanitize(sym):
    dst = ""
    for char in sym:
        if char.isalpha() or char.isdigit() or char == '_':
            dst+=char
        else:
            dst+='_'
    return dst

class Compiler:
    def __init__(self):
        self.__string__ = ""
        self.indent = 0
    def push_raw(self, data):
        self.__string__+=data
    def state(self, statement):
        self.push_raw(self.indent * "\t" + statement + ";\n")
    def state_nocolon(self, statement):
        self.push_raw(self.indent * "\t" + statement)
    def enter_scope(self, expr=None):
        if expr == None:
            self.push_raw(" {\n")
            self.indent+=1
        else:
            self.push_raw(self.indent * "\t" + expr)
            self.push_raw(" {\n")
            self.indent+=1
    def exit_scope(self, use_colon=True):
        self.indent-=1
        if use_colon:
            self.state("}")
        else:
            self.state_nocolon("}\n")
    def write(self, name):
        with open(name, "w") as file:
            file.write(self.__string__)

def target_has_variable(target: Dict, variable: str):
    for var in target["variables"]:
        if target["variables"][var][0] == variable:
            return True
    return False

def get_costume_index(target: Dict, costume: str):
    for costume_index in range(len(target["costumes"])):
        if target["costumes"][costume_index]["name"] == costume:
            return costume_index
    raise IndexError  

def compile_scoped_expr(target: Dict, stage_target: Dict, current: Dict, output: Compiler):
    while True:
        
        # Looks blocks.
        if "looks_switchbackdropto" == current["opcode"]:
            costume_name = target["blocks"][current["inputs"]["BACKDROP"][1]]["fields"]["BACKDROP"][0]
            if target["isStage"]:
                output.state("this->switch_costume(" + str(get_costume_index(target, costume_name)) + ")")
            else:
                output.state("scratch::app()->stage()->switch_costume(" + str(get_costume_index(stage_target, costume_name)) + ")")
        elif "looks_switchcostumeto" == current["opcode"]:
            costume_name = target["blocks"][current["inputs"]["COSTUME"][1]]["fields"]["COSTUME"][0]
            output.state("this->switch_costume(" + str(get_costume_index(target, costume_name)) + ")")
        elif "looks_nextcostume" == current["opcode"]:
            output.state("this->next_costume()")
        elif "looks_show" == current["opcode"]:
            output.state("this->set_visible(true)")
        elif "looks_hide" == current["opcode"]:
            output.state("this->set_visible(false)")
        # Sound blocks.
        elif "sound_play" == current["opcode"]:
            menu = target["blocks"][current["inputs"]["SOUND_MENU"][1]]
            sound_name = menu["fields"]["SOUND_MENU"][0]
            output.state("scratch::app()->play_sound(this->"+sound_name+")")
        elif "sound_playuntildone" == current["opcode"]:
            menu = target["blocks"][current["inputs"]["SOUND_MENU"][1]]
            sound_name = menu["fields"]["SOUND_MENU"][0]
            output.state("scratch::app()->play_sound(this->"+sound_name+")")
        # Motion blocks.
        elif "motion_turnright" == current["opcode"]:
            angle = current["inputs"]["DEGREES"][1][1]
            output.state("this->set_rotation("+angle+")")
        # Control blocks.
        elif "control_wait" == current["opcode"]:
            output.state("std::this_thread::sleep_for("+current["inputs"]["DURATION"][1][1]+"s)")
        elif "control_repeat" == current["opcode"]:
            output.enter_scope("for (int x = 0; x < "+current["inputs"]["TIMES"][1][1]+"; x++)")
            compile_scoped_expr(target, stage_target, target["blocks"][current["inputs"]["SUBSTACK"][1]], output)
            output.exit_scope(False)
        elif "control_if_else" == current["opcode"]:
            output.state_nocolon("if (")
            compile_scoped_expr(target, stage_target, target["blocks"][current["inputs"]["CONDITION"][1]], output)
            output.push_raw(")")
            output.enter_scope()
            compile_scoped_expr(target, stage_target, target["blocks"][current["inputs"]["SUBSTACK"][1]], output)
            output.exit_scope(False)
            
            output.enter_scope("else")
            compile_scoped_expr(target, stage_target, target["blocks"][current["inputs"]["SUBSTACK2"][1]], output)
            output.exit_scope(False)
        elif "control_if" == current["opcode"]:
            output.state_nocolon("if (")
            compile_scoped_expr(target, stage_target, target["blocks"][current["inputs"]["CONDITION"][1]], output)
            output.push_raw(")")
            output.enter_scope()
            compile_scoped_expr(target, stage_target, target["blocks"][current["inputs"]["SUBSTACK"][1]], output)
            output.exit_scope(False)
        elif "control_forever" == current["opcode"]:
            output.enter_scope("while (true)")
            compile_scoped_expr(target, stage_target, target["blocks"][current["inputs"]["SUBSTACK"][1]], output)
            output.exit_scope(False)
        elif "control_stop" == current["opcode"]:
            if current["fields"]["STOP_OPTION"][0] != "other scripts in sprite":
                output.state("return")
            else:
                output.state("// terminate_other_scripts()")
        # Data blocks.
        elif "data_setvariableto" == current["opcode"]:
            var_hash = current["fields"]["VARIABLE"][0]
            if target_has_variable(target, var_hash):
                output.state("this->"+current["fields"]["VARIABLE"][0]+" = "+current["inputs"]["VALUE"][1][1]+"")
            else:
                output.state("dynamic_pointer_cast<"+stage_target["name"]+">(scratch::app()->stage())->"+current["fields"]["VARIABLE"][0]+" = "+current["inputs"]["VALUE"][1][1]+"")
        # Event handling.
        elif "event_broadcast" == current["opcode"]:
            output.state("scratch::app()->event_listener()->broadcast_event_"+current["inputs"]["BROADCAST_INPUT"][1][1]+"()")
        # Sensor blocks.
        elif "sensing_mousedown" == current["opcode"]:
            output.push_raw("(scratch::app()->mouse_down())")
        elif "sensing_touchingobject" == current["opcode"]:
            menu = target["blocks"][current["inputs"]["TOUCHINGOBJECTMENU"][1]]
            if menu["fields"]["TOUCHINGOBJECTMENU"][0] == "_mouse_":
                output.push_raw("(scratch::app()->mouse_touching(this))")
            else:
                object_name = cxx_sanitize(menu["fields"]["TOUCHINGOBJECTMENU"][0])
                output.push_raw("(scratch::app()->get_" + object_name + "()->touching(this))")
        # Operators.
        elif "operator_equals" == current["opcode"]:
            if current["inputs"]["OPERAND1"][0] == 3:
                var_hash = current["inputs"]["OPERAND1"][1][1]
                if target_has_variable(target, var_hash):
                    output.push_raw("(this->"+current["inputs"]["OPERAND1"][1][1])
                else:
                    output.push_raw("(dynamic_pointer_cast<"+stage_target["name"]+">(scratch::app()->stage())->"+current["inputs"]["OPERAND1"][1][1])
            else:
                output.push_raw(current["inputs"]["OPERAND1"][1][1])

            output.push_raw(" == ")

            if current["inputs"]["OPERAND2"][0] == 3:
                var_hash = current["inputs"]["OPERAND2"][1][1]
                if target_has_variable(target, var_hash):
                    output.push_raw("this->"+current["inputs"]["OPERAND2"][1][1]+")")
                else:
                    output.push_raw("(dynamic_pointer_cast<"+stage_target["name"]+">(scratch::app()->stage())->"+current["inputs"]["OPERAND2"][1][1]+")")
            else:
                output.push_raw(current["inputs"]["OPERAND2"][1][1]+")")
        else:
            print("Unimplemented opcode: " + current["opcode"])
        
        # If the next block is null, abort.
        if current["next"] is None:
            break
        
        # Go to the next block.
        current = target["blocks"][current["next"]]

if __name__ == "__main__":
    
    with zipfile.ZipFile('project.sb3', 'r') as sb3:
        with sb3.open('project.json') as project_file:
            project = json.load(project_file)
    
    # Sort all targets by layer order.
    project["targets"] = sorted(project["targets"], key=lambda target: target['layerOrder'])
    
    stage_target = None
    
    # Isolate the stage target.
    for target in project["targets"]:
        if target["isStage"]:
            stage_target = target

    if stage_target == None:
        print("[WARN] Failed to find Stage target.")
    
    backdrop_switches = {}
    
    for target in project["targets"]:
        # Since we handle the stage target later, we simply ignore it for now.
        if target["isStage"]:
            continue
        
        output_header = Compiler()
        output_source = Compiler()
        output_header.push_raw('#pragma once\n#include <chrono>\n#include <thread>\n#include "runtime/Target.hpp"\n#include "runtime/App.hpp"\n#include "runtime/Variable.hpp"\n#include "runtime/Sound.hpp"\n#include "runtime/EventListener.hpp"\n\nusing namespace std::chrono_literals;\n\n')
        
        output_source.push_raw('#include "'+cxx_sanitize(target["name"])+'.hxx"\n#include "'+stage_target["name"]+'.hxx"\n\n')

        print("Compiling " + target["name"] + "...")
        
        # Start by declaring a class with the name of the target, and specifying that it inherits from scratch::Target.
        output_header.enter_scope("class " + cxx_sanitize(target["name"]) + " : public scratch::Target")
        output_header.push_raw("public:\n")
        
        constructor_initializer_list = []
        
        # Declare all associated sounds and sprite-local variables as members, and collect constructor calls into the constructor initializer list for the sprite class.
        for sound in target["sounds"]:
            output_header.state("scratch::Sound " + cxx_sanitize(sound["name"]))
            constructor_initializer_list.append(cxx_sanitize(sound["name"])+'("'+sound["md5ext"]+'")')
        for var in target["variables"]:
            output_header.state("scratch::Variable " + cxx_sanitize(target["variables"][var][0]))
            constructor_initializer_list.append(cxx_sanitize(target["variables"][var][0])+'('+str(target["variables"][var][1])+')')
        
        output_header.state(cxx_sanitize(target["name"])+"()")
        
        # Emit code for the sprite's constructor.
        if len(constructor_initializer_list) == 0:
            output_source.enter_scope(cxx_sanitize(target["name"])+"::"+cxx_sanitize(target["name"])+"()")
        else:
            output_source.enter_scope(cxx_sanitize(target["name"])+"::"+cxx_sanitize(target["name"])+"() : "+", ".join(constructor_initializer_list))
        output_source.state('this->setup("'+cxx_sanitize(target["name"])+'", '+str(target["x"])+', '+str(target["y"])+', '+str(target["size"] / 100)+', '+str(target["visible"]).lower()+')')
        for costume in target["costumes"]:
            output_source.state_nocolon('// ' + costume["name"] + '\n')
            output_source.state('this->costumes.emplace_back("'
            +costume["md5ext"].replace("svg", "png")
            +'", '
            +str(costume["bitmapResolution"])
            +', '
            +str(costume["rotationCenterX"])
            +', '
            +str(costume["rotationCenterY"])
            +')')
        output_source.exit_scope()
        
        # Collect all entry points into an array.
        functions = []
        for block_hash in target["blocks"]:    
            block = target["blocks"][block_hash]
            if "event_when" in block["opcode"]:
                functions.append(block_hash)
        
        # Iterate over each sprite's functions.
        for function in functions:
            # Get the first opcode in the function.
            start = target["blocks"][function]
            
            # The first opcode must be of 'event' type, so we match against those for our output C++ symbol.
            if start["opcode"] == "event_whenbackdropswitchesto":
                backdrop_name = start["fields"]["BACKDROP"][0];
                backdrop_switches[backdrop_name] = (backdrop_switches[backdrop_name] if backdrop_name in backdrop_switches else []) + [cxx_sanitize(target["name"])]
                output_header.state("void when_backdrop_switches_to_" + start["fields"]["BACKDROP"][0]+"()")
                output_source.enter_scope("void "+cxx_sanitize(target["name"])+"::when_backdrop_switches_to_" + start["fields"]["BACKDROP"][0]+"()")
            elif start["opcode"] == "event_whenbroadcastreceived":
                output_header.state("void recieve_" + start["fields"]["BROADCAST_OPTION"][0]+"()")
                output_source.enter_scope("void "+cxx_sanitize(target["name"])+"::recieve_" + start["fields"]["BROADCAST_OPTION"][0]+"()")
            elif start["opcode"] == "event_whenflagclicked":
                output_header.state("void green_flag()")
                output_source.enter_scope("void "+cxx_sanitize(target["name"])+"::green_flag()")
            elif start["opcode"] == "event_whenthisspriteclicked":
                output_header.state("void clicked()")
                output_source.enter_scope("void "+cxx_sanitize(target["name"])+"::clicked()")
            
            # Get the first ACTUAL opcode in the function.
            first_node = target["blocks"][start["next"]]

            # Compile each opcode recursively.
            compile_scoped_expr(target, stage_target, first_node, output_source)
            
            # Emit a '};' to signify the end of the function.
            output_source.exit_scope()

        # Emit a '};' to signify the end of the class definition.
        output_header.exit_scope()
        
        # Save compiled sprite code to cxx file.
        output_header.write("../sprites/"+cxx_sanitize(target["name"])+".hxx")
        output_source.write("../sprites/"+cxx_sanitize(target["name"])+".cxx")
    
    # Now, we compile the Stage target. Just like above, we start by instantiating Compilers for both the header and source files.
    stage_header = Compiler()
    stage_source = Compiler()

    stage_header.push_raw('#pragma once\n#include <chrono>\n#include <thread>\n#include "runtime/Target.hpp"\n#include "runtime/App.hpp"\n#include "runtime/Variable.hpp"\n#include "runtime/Sound.hpp"\n#include "runtime/EventListener.hpp"\n\nusing namespace std::chrono_literals;\n\n')
    stage_source.push_raw('#include "'+cxx_sanitize(stage_target["name"])+'.hxx"\n\n')

    for target_list in backdrop_switches.values():
        for target in target_list:
            stage_source.push_raw('#include "'+target+'.hxx"\n')

    print("Compiling " + stage_target["name"] + "...")

    # Start by declaring a class with the name of the stage_target, and specifying that it inherits from scratch::Target.
    stage_header.enter_scope("class " + cxx_sanitize(stage_target["name"]) + " : public scratch::Target")
    stage_header.push_raw("public:\n")

    constructor_initializer_list = []

    # Declare all associated sounds and sprite-local variables as members, and collect constructor calls into the constructor initializer list for the sprite class.
    for sound in stage_target["sounds"]:
        stage_header.state("scratch::Sound " + cxx_sanitize(sound["name"]))
        constructor_initializer_list.append(cxx_sanitize(sound["name"])+'("'+sound["md5ext"]+'")')
    for var in stage_target["variables"]:
        stage_header.state("scratch::Variable " + cxx_sanitize(stage_target["variables"][var][0]))
        constructor_initializer_list.append(cxx_sanitize(stage_target["variables"][var][0])+'('+str(stage_target["variables"][var][1])+')')

    stage_header.state(cxx_sanitize(stage_target["name"])+"()")

    # Emit code for the sprite's constructor.
    if len(constructor_initializer_list) == 0:
        stage_source.enter_scope(cxx_sanitize(stage_target["name"])+"::"+cxx_sanitize(stage_target["name"])+"()")
    else:
        stage_source.enter_scope(cxx_sanitize(stage_target["name"])+"::"+cxx_sanitize(stage_target["name"])+"() : "+", ".join(constructor_initializer_list))
    for costume in stage_target["costumes"]:
        stage_source.state_nocolon('// ' + costume["name"] + '\n')
        stage_source.state('this->costumes.emplace_back("'
        +costume["md5ext"].replace("svg", "png")
        +'", '
        +str(costume["bitmapResolution"])
        +', '
        +str(costume["rotationCenterX"])
        +', '
        +str(costume["rotationCenterY"])
        +')')
    stage_source.exit_scope()

    # Collect all entry points into an array.
    functions = []
    for block_hash in stage_target["blocks"]:    
        block = stage_target["blocks"][block_hash]
        if "event_when" in block["opcode"]:
            functions.append(block_hash)

    if backdrop_switches:
        stage_header.state('virtual void switch_costume(int index) override\n')
        stage_source.enter_scope('void '+cxx_sanitize(stage_target["name"])+'::switch_costume(int index)')
        handle_index = 0
        for backdrop, target_list in backdrop_switches.items():
            stage_source.enter_scope("if (index == "+str(get_costume_index(stage_target, backdrop))+")")
            for target in target_list:
                stage_source.state('std::thread handle'+str(handle_index)+'([] { dynamic_pointer_cast<'+target+'>(scratch::app()->m_targets.at("'+target+'"))->when_backdrop_switches_to_'+backdrop+"(); })")
                stage_source.state("handle"+str(handle_index)+".detach()")
                handle_index+=1
            stage_source.exit_scope()
        stage_source.exit_scope()

    # Iterate over each sprite's functions.
    for function in functions:
        # Get the first opcode in the function.
        start = stage_target["blocks"][function]
        
        # The first opcode must be of 'event' type, so we match against those for our output C++ symbol.
        if start["opcode"] == "event_whenbackdropswitchesto":
            backdrop_name = start["fields"]["BACKDROP"][0];
            backdrop_switches[backdrop_name] = (backdrop_switches[backdrop_name] if backdrop_name in backdrop_switches else []) + [cxx_sanitize(stage_target["name"])]
            stage_header.state("void when_backdrop_switches_to_" + start["fields"]["BACKDROP"][0]+"()")
            stage_source.enter_scope("void "+cxx_sanitize(stage_target["name"])+"::when_backdrop_switches_to_" + start["fields"]["BACKDROP"][0]+"()")
        elif start["opcode"] == "event_whenbroadcastreceived":
            stage_header.state("void recieve_" + start["fields"]["BROADCAST_OPTION"][0]+"()")
            stage_source.enter_scope("void "+cxx_sanitize(stage_target["name"])+"::recieve_" + start["fields"]["BROADCAST_OPTION"][0]+"()")
        elif start["opcode"] == "event_whenflagclicked":
            stage_header.state("void green_flag()")
            stage_source.enter_scope("void "+cxx_sanitize(stage_target["name"])+"::green_flag()")
        elif start["opcode"] == "event_whenthisspriteclicked":
            stage_header.state("void clicked()")
            stage_source.enter_scope("void "+cxx_sanitize(stage_target["name"])+"::clicked()")
        
        # Get the first ACTUAL opcode in the function.
        first_node = stage_target["blocks"][start["next"]]

        # Compile each opcode recursively.
        compile_scoped_expr(stage_target, stage_target, first_node, stage_source)
        
        # Emit a '};' to signify the end of the function.
        stage_source.exit_scope()

    # Emit a '};' to signify the end of the class definition.
    stage_header.exit_scope()

    # Save compiled sprite code to cxx file.
    stage_header.write("../sprites/"+cxx_sanitize(stage_target["name"])+".hxx")
    stage_source.write("../sprites/"+cxx_sanitize(stage_target["name"])+".cxx")


    # output.write("../transpiled.cxx")