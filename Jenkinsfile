G_giturl = "git@github.com:tonlabs/TON-Compiler.git"
G_gitcred = "LaninSSHgit"
G_container = "alanin/container-llvm:latest"
G_buildstatus = "NotSet"
G_teststatus = "NotSet"
G_llvmstatus = "NotSet"
G_clangstatus = "NotSet"
G_workdir = "/opt/work"
G_ramdir = "/media/ramdisk/toolchain"
G_dirdouble = G_ramdir + ":" + G_workdir
C_PROJECT = "NotSet"
C_COMMITER = "NotSet"
C_HASH = "NotSet"
C_TEXT = "NotSet"
def getVar(Gvar) {return Gvar}
def updateGithubCommitStatus(build) {
    step([
        $class: 'GitHubCommitStatusSetter',
        reposSource: [$class: "ManuallyEnteredRepositorySource", url: C_GITURL],
        commitShaSource: [$class: "ManuallyEnteredShaSource", sha: C_GITCOMMIT],
        errorHandlers: [[$class: 'ShallowAnyErrorHandler']],
        statusResultSource: [$class: 'ConditionalStatusResultSource',results: [
        [$class: 'AnyBuildResult', state: build.currentResult, message: build.description]]]])
}
// Deploy chanel
DiscordURL = "https://discordapp.com/api/webhooks/496992026932543489/4exQIw18D4U_4T0H76bS3Voui4SyD7yCQzLP9IRQHKpwGRJK1-IFnyZLyYzDmcBKFTJw"
pipeline {
    agent {
        docker {
        image G_container
            args '--network proxy_nw -v G_dirdouble'
        }
    }
    environment {
    WORKDIR = getVar(G_workdir)
    }
    stages {
        stage('Initialise') {
            steps {
                script {
                    G_gitproject = G_giturl.substring(0,G_giturl.length()-4)
                    properties([[$class: 'GithubProjectProperty',
                    projectUrlStr: G_gitproject]])
                    G_gitbranch = sh (script: 'echo ${BRANCH_NAME}',returnStdout: true).trim()
                }
            }
        }
        stage('Build') {
            steps{
                script {
                    C_TEXT = sh (script: 'git show -s --format=%s ${GIT_COMMIT}',returnStdout: true).trim()
                    C_AUTHOR = sh (script: 'git show -s --format=%an ${GIT_COMMIT}',returnStdout: true).trim()
                    C_COMMITER = sh (script: 'git show -s --format=%cn ${GIT_COMMIT}',returnStdout: true).trim()
                    C_HASH = sh (script: 'git show -s --format=%h ${GIT_COMMIT}',returnStdout: true).trim()
                    C_PROJECT = G_giturl.substring(15,G_giturl.length()-4)
                    C_GITURL = sh (script: 'echo ${GIT_URL}',returnStdout: true).trim()
                    C_GITCOMMIT = sh (script: 'echo ${GIT_COMMIT}',returnStdout: true).trim()
                }
                    sh 'rm -rf ${WORKDIR}/llvm/*'
                    sh 'cp -R llvm/* ${WORKDIR}/llvm'
                    sh 'mkdir ${WORKDIR}/llvm/build'
                    sh 'cp /opt/ninja ${WORKDIR}/llvm/build'
                    sh 'cd ${WORKDIR}/llvm/build && cmake -G "Ninja" \
                    -DCMAKE_C_COMPILER=clang \
                    -DCMAKE_CXX_COMPILER=clang++ \
                    -DLLVM_TARGETS_TO_BUILD="X86" \
                    -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="TVM" \
                    -DBUILD_SHARED_LIBS=On \
                    -DLLVM_OPTIMIZED_TABLEGEN=On \
                    -DLLVM_USE_LINKER=lld ..'
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
                    sh 'cd ${WORKDIR}/llvm/build && cmake --build . --target check'
            }
            post {
                success {script{G_teststatus = "success"}}
                failure {script{G_teststatus = "failure"}}
            }
        }
    }
    post {
        always {
            script {
                currentBuild.description = C_TEXT
                string DiscordFooter = "Build duration is " + currentBuild.durationString
                DiscordTitle = "Job ${JOB_NAME} from GitHub " + C_PROJECT
                DiscordDescription = C_COMMITER + " pushed commit " + C_HASH + " by " + C_AUTHOR + " with a message '" + C_TEXT + "'" + "\n" \
                + "Build number ${BUILD_NUMBER}" + "\n" \
                + "Build: **" + G_buildstatus + "**" + "\n" \
                + "Clang: **" + G_clangstatus + "**" + "\n" \
                + "Llvm: **" + G_llvmstatus + "**" + "\n" \
                + "Tests: **" + G_teststatus + "**" + "\n"
                discordSend description: DiscordDescription, footer: DiscordFooter, link: JOB_DISPLAY_URL, successful: currentBuild.resultIsBetterOrEqualTo('SUCCESS'), title: DiscordTitle, webhookURL: DiscordURL
            }
        }
        failure {
            step([$class: 'GitHubIssueNotifier',
                  issueAppend: true,
                  issueTitle: '$JOB_NAME $BUILD_DISPLAY_NAME failed'])
        }
    }
}