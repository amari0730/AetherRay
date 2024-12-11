import json
import subprocess

# path = '/Users/mohammadhijaz/Desktop/cs-1230/final-project/scenefiles/intersect/required/falling_sphere.json'
path = "/Users/davideskilson/other/browncs/CSCI-1230/final-project/AetherRay/scenefiles/intersect/required/falling_sphere.json"
with open(path, "r") as f:
    data = json.load(f)


# print(data['groups'][1]['groups'][0]['translate'])
# print(data['groups'][1]['groups'][0][''])
for frame in range(0, 11):
    print("creating new frame JSON frame i: ", frame)
    with open("jsonFrames/frame" + str(frame) + ".json", "w") as f:
        json.dump(data, f)
    data["groups"][1]["groups"][0]["translate"][1] -= 0.2
    data["groups"][1]

# ini_str = """[IO]
#     scene = /Users/mohammadhijaz/Desktop/cs-1230/final-project/jsonFrames/
#     output = /Users/mohammadhijaz/Desktop/cs-1230/final-project/outputFrames/

# [Canvas]
#     width = 1024
#     height = 768

# [Feature]
#     shadows = false
#     reflect = false
#     refract = false
#     texture = false
#     parallel = false
#     super-sample = false
#     num-samples = 1
#     post-process = false
#     acceleration = false
#     depthoffield = false
# """
ini_str = """[IO]
    scene = /Users/davideskilson/other/browncs/CSCI-1230/final-project/AetherRay/jsonFrames/
    output = /Users/davideskilson/other/browncs/CSCI-1230/final-project/AetherRay/outputFrames/

[Canvas]
    width = 1024
    height = 768

[Feature]
    shadows = false
    reflect = false
    refract = false
    texture = false
    parallel = false
    super-sample = false
    num-samples = 1
    post-process = false
    acceleration = false
    depthoffield = false
"""
lines = ini_str.splitlines()
for frame in range(0, 11):
    print("creating new frame ini frame i: ", frame)
    cur_lines = lines
    cur_lines[1] = (
        # " scene = /Users/mohammadhijaz/Desktop/cs-1230/final-project/jsonFrames/"
        " scene = /Users/davideskilson/other/browncs/CSCI-1230/final-project/AetherRay/jsonFrames/"
        + "frame"
        + str(frame)
        + ".json"
    )
    cur_lines[2] = (
        # " output = /Users/mohammadhijaz/Desktop/cs-1230/final-project/outputFrames/"
        " output = /Users/davideskilson/other/browncs/CSCI-1230/final-project/AetherRay/outputFrames/"
        + "frame"
        + str(frame)
        + ".png"
    )
    # print(cur_line)
    ini_to_str = ""
    for line in cur_lines:
        ini_to_str += line
        ini_to_str += "\n"
    with open("iniFrames/frame" + str(frame) + ".ini", "w") as f:
        f.write(ini_to_str)

# executable_path = "build/build-final-project-Qt_6_7_3_for_macOS-Release/projects_ray"
executable_path = (
    "build/build-project-aether-ray-Qt_6_7_3_for_macOS-Release/project_aether_ray"
)
# argument = "/Users/mohammadhijaz/Desktop/cs-1230/final-project/iniFrames/"
argument = (
    "/Users/davideskilson/other/browncs/CSCI-1230/final-project/AetherRay/iniFrames/"
)
for frame in range(0, 11):
    cur_argument = argument
    cur_argument += "frame" + str(frame) + ".ini"
    # print(cur_argument)
    print("generating frame: ", frame)
    result = subprocess.run(
        [executable_path, cur_argument], capture_output=True, text=True
    )
    # print(result.stderr)
    print("generated frame ", frame)

# print("Standard Output:")
# print(result.stdout)

# print("Standard Error:")
# print(result.stderr)
# print('finished running')
