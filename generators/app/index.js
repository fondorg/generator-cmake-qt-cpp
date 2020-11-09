const Generator = require('yeoman-generator');

module.exports = class extends Generator {
    initializing() {
        this.log("Initializing...");
    }

    async prompting() {
        this.answers = await this.prompt([
            {
                type: "input",
                name: "project",
                message: "Your project name",
                default: this.appname.replace(/\s+/g, '_')
            },
            {
                type: "input",
                name: "windowTitle",
                message: "Application window title",
                default: this.appname.charAt(0).toUpperCase() + this.appname.slice(1)
            },
            {
                type: "input",
                name: "organization",
                message: "Your organization (required when saving settings)",
                default: "MyOrganization",
                store: true
            },
            {
                type: "confirm",
                name: "opencv",
                message: "Enable OpenCV library?",
                store: true
            }
        ])
    }

    writing() {
        this.fs.copyTpl(
            this.templatePath('CMakeLists.txt'),
            this.destinationPath('CMakeLists.txt'),
            {
                project: this.answers.project,
                opencv: this.answers.opencv
            }
        );
        this.fs.copyTpl(
            this.templatePath('.gitignore'),
            this.destinationPath('.gitignore'),
        );
        this.fs.copyTpl(
            this.templatePath('src/main.cpp'),
            this.destinationPath('src/main.cpp'),
        );
        this.fs.copyTpl(
            this.templatePath('src/main_window.h'),
            this.destinationPath('src/main_window.h'),
            {
                opencv: this.answers.opencv
            }
        );
        this.fs.copyTpl(
            this.templatePath('src/main_window.cpp'),
            this.destinationPath('src/main_window.cpp'),
            {
                project: this.answers.project,
                windowTitle: this.answers.windowTitle,
                organization: this.answers.organization
            }
        );
        this.fs.copyTpl(
            this.templatePath('src/mainwindow.ui'),
            this.destinationPath('src/mainwindow.ui'),
        );
    }

};