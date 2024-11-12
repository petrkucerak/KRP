import re
import os


def parse_makefile_log(log_file):
    undefined_funcs = []
    c_source_files = set()

    # Pattern to match undefined reference errors in the log file
    undefined_pattern = re.compile(r": undefined reference to `(\w+)'")

    # Read the log file and extract all undefined function names
    with open(log_file, 'r') as file:
        for line in file:
            match = undefined_pattern.search(line)
            if match:
                undefined_func = match.group(1)
                undefined_funcs.append(undefined_func)

    # Paths to search for .c files (Adjust these paths based on project structure)
    search_paths = [
        "../../CM7/Core/Src",
        "../../Drivers/STM32H7xx_HAL_Driver/Src",
        "../../Drivers/BSP/STM32H747I-DISCO",
        "../../Common/Src"
    ]

    # Search for each undefined function in the .c files in the specified directories
    for path in search_paths:
        for root, _, files in os.walk(path):
            for file in files:
                if file.endswith(".c"):
                    file_path = os.path.join(root, file)
                    with open(file_path, 'r') as src_file:
                        content = src_file.read()
                        # Check if any of the undefined functions are in this .c file
                        if any(func in content for func in undefined_funcs):
                            c_source_files.add(file_path)

    # Generate the output format for C_SOURCES
    c_sources_text = "# C sources\nC_SOURCES =  \\\n"
    c_sources_text += " \\\n".join(sorted(c_source_files)) + " \\"

    return c_sources_text


# Specify the path to the makefile log file
log_file_path = "makefile.log"
output = parse_makefile_log(log_file_path)
print(output)
