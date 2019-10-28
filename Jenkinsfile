G_giturl = "git@github.com:tonlabs/TON-Compiler.git"
G_gitcred = "LaninSSHgit"
G_container = "alanin/container-llvm:latest"
G_promoted_branch = "origin/master"
G_dockerimage = "NotSet"
G_buildstatus = "NotSet"
G_clangstatus = "NotSet"
G_llvmstatus = "NotSet"
G_teststatus = "NotSet"
G_Wbuildstatus = "NotSet"
G_Wclangstatus = "NotSet"
G_Wllvmstatus = "NotSet"
G_Wteststatus = "NotSet"
G_workdir = "/opt/work/${GIT_COMMIT}.${BUILD_NUMBER}"
G_ramdir = "/media/ramdisk/toolchain"
C_PROJECT = "NotSet"
C_COMMITER = "NotSet"
C_HASH = "NotSet"
C_TEXT = "NotSet"
def getVar(Gvar) {return Gvar}
// Deploy chanel
DiscordURL = "https://discordapp.com/api/webhooks/496992026932543489/4exQIw18D4U_4T0H76bS3Voui4SyD7yCQzLP9IRQHKpwGRJK1-IFnyZLyYzDmcBKFTJw"

pipeline {
    parameters {
        booleanParam (
            defaultValue: false,
            description: 'Promote image built to be used as latest',
            name : 'FORCE_PROMOTE_LATEST'
        )
    }
    agent none
    options {
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '20')
        disableConcurrentBuilds()
        }
    environment {
        WORKDIR = getVar(G_workdir)
        RAMDIR = getVar(G_ramdir)
    }
    stages {
        stage('Processing...') {
        parallel {
            stage('On Linux') {
                agent {
                    docker {
                        image G_container
                        args '--network proxy_nw -v ${RAMDIR}:${WORKDIR}'
                    }
                }
                stages {
                    stage('Initialise') {
                        steps {
                            script {
                                G_gitproject = G_giturl.substring(0,G_giturl.length()-4)
                                properties([[$class: 'GithubProjectProperty',
                                projectUrlStr: G_gitproject]])
                                G_gitbranch = sh (script: 'echo ${BRANCH_NAME}',returnStdout: true).trim()
                                G_clangformat = sh (script: 'git-clang-format --diff HEAD~',returnStdout: true).trim()
                                C_TEXT = sh (script: 'git show -s --format=%s ${GIT_COMMIT}',returnStdout: true).trim()
                                C_AUTHOR = sh (script: 'git show -s --format=%an ${GIT_COMMIT}',returnStdout: true).trim()
                                C_COMMITER = sh (script: 'git show -s --format=%cn ${GIT_COMMIT}',returnStdout: true).trim()
                                C_HASH = sh (script: 'git show -s --format=%h ${GIT_COMMIT}',returnStdout: true).trim()
                                C_PROJECT = G_giturl.substring(15,G_giturl.length()-4)
                                C_GITURL = sh (script: 'echo ${GIT_URL}',returnStdout: true).trim()
                                C_GITCOMMIT = sh (script: 'echo ${GIT_COMMIT}',returnStdout: true).trim()
                                println (G_clangformat)
                                switch(G_clangformat) {
                                    case ["clang-format did not modify any files", "no modified files to format"]:
                                    break
                                    default:
                                    G_clangformat = "files was modified by clang-format"
                                    break
                                }
                            }
                        }
                    }
                    stage('Configure') {
                        environment {
                            HOME = "$WORKSPACE"
                        }
                        steps{
                            script {
                                dir ('sdk-emulator') {
                                    git branch: 'dev', credentialsId: G_gitcred, url: 'git@github.com:tonlabs/sdk-emulator.git'
                                }
                                sshagent([G_gitcred]) {
                                    sh '''
                                    wget  https://sh.rustup.rs -O rust.sh && bash rust.sh -y && rm rust.sh
                                    export PATH="${HOME}/.cargo/bin:${PATH}"
                                    rustup component add rustfmt-preview && rustup target add i686-unknown-linux-gnu
                                    cd ${WORKSPACE}/sdk-emulator/tvm
                                    cargo build
                                    '''
                                }
                                sh 'rm -rf ${WORKDIR}/llvm/*'
                                sh 'cp -R llvm/* ${WORKDIR}/llvm'
                                sh 'mkdir ${WORKDIR}/llvm/build'
                                sh 'cd ${WORKDIR}/llvm/build && cmake -G "Ninja" \
                                -DCMAKE_C_COMPILER=clang \
                                -DCMAKE_CXX_COMPILER=clang++ \
                                -DLLVM_TARGETS_TO_BUILD="X86" \
                                -DBUILD_SHARED_LIBS=On \
                                -DLLVM_OPTIMIZED_TABLEGEN=On \
                                -DLLVM_USE_LINKER=lld ..'
                            }
                        }
                        post {
                            success {script{G_buildstatus = "success"}}
                            failure {script{G_buildstatus = "failure"}}
                        }
                    }
                    stage('Build clang') {
                        steps{
                            sh 'cd ${WORKDIR}/llvm/build && cmake --build . --target clang'
                        }
                        post {
                            success {script{G_clangstatus = "success"}}
                            failure {script{G_clangstatus = "failure"}}
                        }
                    }
                    stage('Build llc') {
                        steps{
                            sh 'cd ${WORKDIR}/llvm/build && cmake --build . --target llc'
                        }
                        post {
                            success {script{G_llvmstatus = "success"}}
                            failure {script{G_llvmstatus = "failure"}}
                        }
                    }
                    stage('Tests') {
                        steps{
                            sh 'cd ${WORKDIR}/llvm/build && cmake --build . --target check-all'
                        }
                        post {
                            success {script{G_teststatus = "success"}}
                            failure {script{G_teststatus = "failure"}}
                        }
                    }
                }
            }
            stage('On Windows'){
                agent { label 'Win01' }
                when {
                    expression {
                        return false
                    }
                }
                stages{
                    stage('Configure'){
                        steps {
                            script {
                                dir('llvm/build'){
                                    deleteDir()
                                    bat label: 'Configure', script: '''
                                    "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat" && cmake -G "Ninja" ^
                                    -DLLVM_TARGETS_TO_BUILD="X86" ^
                                    -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="TVM" ^
                                    -DLLVM_OPTIMIZED_TABLEGEN=On ..
                                    '''
                                }
                            }
                        }
                        post {
                            success {script{G_Wbuildstatus = "success"}}
                            failure {script{G_Wbuildstatus = "failure"}}
                        }
                    }
                    stage('Build clang') {
                        steps{
                            dir('llvm/build'){
                                bat label: 'Clang', script: '''
                                "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat" && cmake^
                                --build . --target clang
                                '''
                            }
                        }
                        post {
                            success {script{G_Wclangstatus = "success"}}
                            failure {script{G_Wclangstatus = "failure"}}
                        }
                    }
                    stage('Build llc') {
                        steps{
                            dir('llvm/build'){
                                bat label: 'LLC', script: '''
                                "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat" && cmake^
                                --build . --target llc
                                '''
                            }
                        }
                        post {
                            success {script{G_Wllvmstatus = "success"}}
                            failure {script{G_Wllvmstatus = "failure"}}
                        }
                    }
                    stage('Tests') {
                        when {
                            expression {
                                GIT_BRANCH = "origin/${BRANCH_NAME}"
                                return GIT_BRANCH == G_promoted_branch || params.FORCE_PROMOTE_LATEST
                            }
                        }
                        steps{
                            dir('llvm/build'){
                                bat label: 'Tests', script: '''
                                "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat" && cmake^
                                --build . --target check-llvm-unit
                                '''
                            }
                        }
                        post {
                            success {script{G_Wteststatus = "success"}}
                            failure {script{G_Wteststatus = "failure"}}
                        }
                    }
                }
            }
            stage('Build docker-image') {
                    agent {
                        node {label 'master'}
                    }
                    stages {
                        stage('Build') {
                            steps {
                                script {
                                    G_dockerimage = "tonlabs/ton_compiler:${GIT_COMMIT}"
                                    app_image = docker.build(
                                        "${G_dockerimage}", 
                                        '--label "git-commit=${GIT_COMMIT}" .'
                                    )
                                }
                            }
                        }
                        stage('Test') { 
                            steps {
                                script {
                                    G_dockerimage = "tonlabs/ton_compiler:${GIT_COMMIT}"
                                    docker.image(G_dockerimage).inside("-u root") {
					    sh 'sh /app/install.sh'
					    sh 'clang-7 --version'
					    sh 'clang --version'
				    }
                                }
                            }
                        }
                        stage('Push docker-image') {
                            steps {
                                script {
                                    docker.withRegistry('', 'dockerhubLanin') {
                                        app_image.push()
                                    }
                                }
                            }
                            post {
                                failure {script{G_buildstatus = "failure"}}
                            }
                        }
                        stage('Test in compiler-kit') {
                            steps {
                                script {
                                    def params = [
                                      [$class: 'StringParameterValue', name: 'dockerimage_ton_compiler', value: "${G_dockerimage}"]
                                    ]
                                    build job : "infrastructure/compilers/master", parameters : params
                                }
                            }
                            post {
                                success {
                                    script{
                                        G_buildstatus = "success"
                                    }
                                }
                                failure {script{G_buildstatus = "failure"}}
                            }
                        }
                    }
                }
        }
        }
	stage ('Tag as latest') {
            when {
                expression {
                    GIT_BRANCH = "origin/${BRANCH_NAME}"
                    return GIT_BRANCH == G_promoted_branch || params.FORCE_PROMOTE_LATEST
                } 
            }
            agent {
                node {label 'master'}
            }
            steps {
                script {
                    docker.withRegistry('', 'dockerhubLanin') {
                        docker.image("${G_dockerimage}").push('latest')
                    }
                }
            }        
        }
    }
    post {
        always {
            node ('master') {
                script {
                    cleanWs notFailBuild: true
                    currentBuild.description = C_TEXT
                    string DiscordFooter = "Build duration is " + currentBuild.durationString
                    DiscordTitle = "Job ${JOB_NAME} from GitHub " + C_PROJECT
                    DiscordDescription = C_COMMITER + " pushed commit " + C_HASH + " by " + C_AUTHOR + " with a message '" + C_TEXT + "'" + "\n" \
                    + "Build number ${BUILD_NUMBER}" + "\n" \
                    + "__**Linux**__\n" \
                    + "Configure: **" + G_buildstatus + "**" + "\n" \
                    + "Clang: **" + G_clangstatus + "**" + "\n" \
                    + "Llvm: **" + G_llvmstatus + "**" + "\n" \
                    + "Tests: **" + G_teststatus + "**" + "\n" \
                    + "Format: **" + G_clangformat + "**\n" \
                    + "__**Windows**__\n" \
                    + "Configure: **" + G_Wbuildstatus + "**" + "\n" \
                    + "Clang: **" + G_Wclangstatus + "**" + "\n" \
                    + "Llvm: **" + G_Wllvmstatus + "**" + "\n" \
                    + "Tests: **" + G_Wteststatus + "**"
                    discordSend description: DiscordDescription, footer: DiscordFooter, link: RUN_DISPLAY_URL, successful: currentBuild.resultIsBetterOrEqualTo('SUCCESS'), title: DiscordTitle, webhookURL: DiscordURL
                }
            }
            node ('Win01') {script{cleanWs notFailBuild: true}}
        }
        failure {
            step([$class: 'GitHubIssueNotifier',
                  issueAppend: true,
                  issueTitle: '$JOB_NAME $BUILD_DISPLAY_NAME failed'])
        }
    }
}

