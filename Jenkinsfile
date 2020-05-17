G_giturl = "git@github.com:tonlabs/TON-Compiler.git"
G_gitcred = "LaninSSHgit"
G_container = "alanin/container-llvm:latest"
G_promoted_branch = "origin/master"
G_dockerimage = "NotSet"
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
          }
          stage('Build & Run LLVM Regression Tests') {
            steps {
              sh 'cd ${WORKDIR}/llvm/build && ninja check-all'
            }
          }
          stage('Build the installation') {
            //when {
            //    expression {
            //        return GIT_BRANCH == 'master' || GIT_BRANCH ==~ '^PR-[0-9]+'
            //    }
            //}
            environment {
              HOME = "$WORKSPACE"
            }
            steps {
              script {
                sh 'rm -rf ${WORKDIR}/llvm/*'
                sh 'cp -R llvm/* ${WORKDIR}/llvm'
                sh 'cp -R cmake/Cache/ton-compiler.cmake ${WORKDIR}/llvm'
                sh 'mkdir ${WORKDIR}/llvm/build'
                sh 'cd ${WORKDIR}/llvm/build &&   \
                cmake -G Ninja                    \
                -DCMAKE_C_COMPILER=clang          \
                -DCMAKE_CXX_COMPILER=clang++      \
                -DLLVM_USE_LINKER=lld             \
                -DCMAKE_INSTALL_PREFIX=../install \
                -C ../ton-compiler.cmake          \
                ..'
                sh 'cd ${WORKDIR}/llvm/build && ninja install-distribution'
                sh 'rm -rf ${WORKDIR}/llvm/build/*'
                sh 'tar -czvf cpp_toolchain.tar.gz ${WORKDIR}/llvm/install'
                sh 'ls ${WORKDIR}/llvm'
                withAWS(credentials: 'CI_bucket_writer', region: 'eu-central-1') {
                  identity = awsIdentity()
                  s3Upload \
                    bucket: 'sdkbinaries-ws.tonlabs.io', \
                    includePathPattern:"${WORKDIR}/llvm/*.gz", path: "tvm_llvm", \
                    workingDir:'.'
                }
              }
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
