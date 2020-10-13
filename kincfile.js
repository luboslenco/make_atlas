let project = new Project('atlas', __dirname);

project.addFile('Sources/**');
project.setDebugDir('Deployment');
project.cpp = true;

resolve(project);
