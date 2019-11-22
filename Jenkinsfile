G_giturl = "git@github.com:tonlabs/TON-Compiler.git"
G_gitcred = "LaninSSHgit"
G_container = "alanin/container-llvm:latest"
G_promoted_branch = "origin/master"
G_dockerimage = "NotSet"
G_buildstatus = "NotSet"
G_clangstatus = "NotSet"
G_llvmstatus = "NotSet"
G_teststatus = "NotSet"
G_workdir = "/opt/work"
G_ramdir = "/media/ramdisk/toolchain"

pipeline {
  parameters {
    booleanParam (
      defaultValue: false,
      description: 'Promote the image built to be used as latest',
      name : 'FORCE_PROMOTE_LATEST'
    )
  }
  agent none
  options {
    buildDiscarder logRotator(artifactDaysToKeepStr: '',
                              artifactNumToKeepStr: '',
                              daysToKeepStr: '',
                              numToKeepStr: '20')
    disableConcurrentBuilds()
    lock('RamDrive')
  }
  environment {
    WORKDIR = "${G_workdir}"
    RAMDIR = "${G_ramdir}"
  }
  stages {
    stage('Processing') {
    when { expression { return !(env.BRANCH_NAME ==~ /^PR-\d+$/) } }
    parallel {
      stage('Linux') {
        agent {
          docker {
            image G_container
            args '--network proxy_nw -v ${RAMDIR}:${WORKDIR}'
          }
        }
        stages {
          stage('Initialize') {
            steps {
              script {
                G_gitproject = G_giturl.substring(0, G_giturl.length() - 4)
                properties([[$class: 'GithubProjectProperty',
                             projectUrlStr: G_gitproject]])
              }
            }
          }
          stage('Configure') {
            environment {
              HOME = "$WORKSPACE"
            }
            steps {
              script {
                sh 'rm -rf ${WORKDIR}/llvm/*'
                sh 'cp -R llvm/* ${WORKDIR}/llvm'
                sh 'mkdir ${WORKDIR}/llvm/build'
                sh 'cd ${WORKDIR}/llvm/build &&  \
                cmake -G Ninja                   \
                -DCMAKE_C_COMPILER=clang         \
                -DCMAKE_CXX_COMPILER=clang++     \
                -DLLVM_TARGETS_TO_BUILD="X86"    \
                -DBUILD_SHARED_LIBS=1            \
                -DCLANG_BUILD_TOOLS=0            \
                -DCLANG_ENABLE_STATIC_ANALYZER=0 \
                -DCLANG_ENABLE_ARCMT=0           \
                -DLLVM_USE_LINKER=lld            \
                ..'
              }
            }
            post {
              success { script { G_buildstatus = "success" } }
              failure { script { G_buildstatus = "failure" } }
            }
          }
          stage('Build & Run Tests') {
            steps{
              sh 'cd ${WORKDIR}/llvm/build && ninja check-all'
            }
            post {
              success { script { G_teststatus = "success" } }
              failure { script { G_teststatus = "failure" } }
            }
          }
        }
      }
      stage('TVM') {
        agent {
          node { label 'master' }
        }
        stages {
          stage('Build') {
            steps {
              script {
                G_dockerimage = "tonlabs/ton_compiler:${GIT_COMMIT}"
                app_image = docker.build("${G_dockerimage}",
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
              failure { script { G_buildstatus = "failure" } }
            }
          }
          stage('Test in compiler-kit') {
            steps {
              script {
                def params = [[
                  $class: 'StringParameterValue',
                  name: 'dockerimage_ton_compiler',
                  value: "${G_dockerimage}"
                ]]
                build job : "infrastructure/compilers/master",
                      parameters : params
              }
            }
            post {
              success {
                script{
                  G_buildstatus = "success"
                }
              }
              failure { script { G_buildstatus = "failure" } }
            }
          }
        }
      } // stage tvm
    } // parallel
    } // stage processing
    stage ('Tag as latest') {
      when {
        expression {
          GIT_BRANCH = "origin/${BRANCH_NAME}"
          return GIT_BRANCH == G_promoted_branch || params.FORCE_PROMOTE_LATEST
        }
      }
      agent {
        node { label 'master' }
      }
      steps {
        script {
          docker.withRegistry('', 'dockerhubLanin') {
            docker.image("${G_dockerimage}").push('latest')
          }
        }
      }
    }
  } // stages
  post {
    always {
      node ('master') { script { cleanWs notFailBuild: true } }
    }
    failure {
      step([$class: 'GitHubIssueNotifier',
            issueAppend: true,
            issueTitle: '$JOB_NAME $BUILD_DISPLAY_NAME failed'])
    }
  }
} // pipeline
