let project = new Project('TP2_Julien');

project.kore = false;
project.cmd = true;

project.addIncludeDir('optick/include')
project.addExclude('optick/src/optick_gpu.vulkan.cpp')
project.addExclude('optick/src/optick_gpu.d3d12.cpp')
project.addFiles('Sources/**','optick/src/**');
project.setDebugDir('./Deployment');

resolve(project);
