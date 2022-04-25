; RUN: llc < %s -march=tvm

; ModuleID = './Console_Debot.opt2.ll'
source_filename = "Console_Debot.cpp"
target datalayout = "e-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm-unknown-unknown"

; Function Attrs: nounwind
define dso_local i257 @main_external(cell, slice) local_unnamed_addr #0 personality i32 (...)* @__gxx_personality_v0 {
  %3 = tail call i257 @llvm.tvm.pushnull() #6
  %4 = tail call cell @llvm.tvm.cast.to.cell(i257 %3) #6
  %5 = tail call builder @llvm.tvm.newc() #6
  %6 = tail call builder @llvm.tvm.stu(i257 2, builder %5, i257 2) #6
  %7 = tail call builder @llvm.tvm.stu(i257 0, builder %6, i257 1) #6
  %8 = tail call builder @llvm.tvm.sti(i257 0, builder %7, i257 8) #6
  %9 = tail call builder @llvm.tvm.stu(i257 0, builder %8, i257 256) #6
  %10 = tail call cell @llvm.tvm.endc(builder %9) #6
  %11 = tail call slice @llvm.tvm.ctos(cell %10) #6
  %12 = tail call { i257, slice } @llvm.tvm.ldu(slice %1, i257 1) #6
  %13 = extractvalue { i257, slice } %12, 0
  %14 = extractvalue { i257, slice } %12, 1
  %15 = icmp eq i257 %13, 0
  br i1 %15, label %19, label %16

; <label>:16:                                     ; preds = %2
  %17 = tail call { slice, slice } @llvm.tvm.ldslice(slice %14, i257 512) #6
  %18 = extractvalue { slice, slice } %17, 1
  br label %19

; <label>:19:                                     ; preds = %16, %2
  %20 = phi slice [ %14, %2 ], [ %18, %16 ]
  %21 = tail call { i257, slice } @llvm.tvm.ldu(slice %20, i257 64) #6
  %22 = extractvalue { i257, slice } %21, 0
  %23 = extractvalue { i257, slice } %21, 1
  %24 = tail call { i257, slice } @llvm.tvm.ldu(slice %23, i257 32) #6
  %25 = extractvalue { i257, slice } %24, 1
  %26 = tail call { i257, slice } @llvm.tvm.ldu(slice %25, i257 32) #6
  %27 = extractvalue { i257, slice } %26, 0
  switch i257 %27, label %717 [
    i257 1756716863, label %28
    i257 1757739143, label %47
    i257 94113135, label %170
    i257 805461396, label %370
    i257 893474671, label %466
    i257 2112671963, label %591
  ]

; <label>:28:                                     ; preds = %19
  %29 = tail call cell @llvm.tvm.get.persistent.data() #6
  %30 = tail call slice @llvm.tvm.ctos(cell %29) #6
  %31 = tail call { i257, slice } @llvm.tvm.ldu(slice %30, i257 1) #6
  %32 = extractvalue { i257, slice } %31, 0
  %33 = icmp eq i257 %32, 0
  br i1 %33, label %34, label %35

; <label>:34:                                     ; preds = %28
  tail call void @llvm.tvm.throw(i257 62) #6
  unreachable

; <label>:35:                                     ; preds = %28
  tail call void @llvm.tvm.accept() #6
  %36 = tail call builder @llvm.tvm.newc() #6
  %37 = tail call builder @llvm.tvm.stu(i257 0, builder %36, i257 1) #6
  %38 = tail call builder @llvm.tvm.stu(i257 0, builder %37, i257 64) #6
  %39 = tail call builder @llvm.tvm.stu(i257 0, builder %38, i257 32) #6
  %40 = tail call builder @llvm.tvm.stdict(cell %4, builder %39) #6
  %41 = tail call builder @llvm.tvm.stu(i257 0, builder %40, i257 1) #6
  %42 = tail call cell @llvm.tvm.endc(builder %37) #6
  %43 = tail call builder @llvm.tvm.stref(cell %42, builder %37) #6
  %44 = tail call cell @llvm.tvm.endc(builder %43) #6
  %45 = tail call builder @llvm.tvm.stref(cell %44, builder %41) #6
  %46 = tail call cell @llvm.tvm.endc(builder %45) #6
  tail call void @llvm.tvm.set.persistent.data(cell %46) #6
  br label %_ZN3tvm12smart_switchILb0E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit

; <label>:47:                                     ; preds = %19
  %48 = tail call cell @llvm.tvm.get.persistent.data() #6
  %49 = tail call slice @llvm.tvm.ctos(cell %48) #6
  %50 = tail call { i257, slice } @llvm.tvm.ldu(slice %49, i257 1) #6
  %51 = extractvalue { i257, slice } %50, 0
  %52 = icmp eq i257 %51, 0
  br i1 %52, label %54, label %53

; <label>:53:                                     ; preds = %47
  tail call void @llvm.tvm.throw(i257 63) #6
  unreachable

; <label>:54:                                     ; preds = %47
  %55 = extractvalue { i257, slice } %50, 1
  %56 = tail call { i257, slice } @llvm.tvm.ldu(slice %55, i257 64) #6
  %57 = extractvalue { i257, slice } %56, 1
  %58 = extractvalue { i257, slice } %56, 0
  %59 = tail call { i257, slice } @llvm.tvm.ldu(slice %57, i257 32) #6
  %60 = extractvalue { i257, slice } %59, 1
  %61 = tail call { cell, slice } @llvm.tvm.lddict(slice %60) #6
  %62 = extractvalue { i257, slice } %59, 0
  %63 = extractvalue { cell, slice } %61, 0
  %64 = extractvalue { cell, slice } %61, 1
  %65 = tail call { i257, slice } @llvm.tvm.ldu(slice %64, i257 1) #6
  %66 = extractvalue { i257, slice } %65, 0
  %67 = extractvalue { i257, slice } %65, 1
  %68 = icmp eq i257 %66, 0
  br i1 %68, label %73, label %69

; <label>:69:                                     ; preds = %54
  %70 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %67) #6
  %71 = extractvalue { slice, slice } %70, 0
  %72 = extractvalue { slice, slice } %70, 1
  br label %73

; <label>:73:                                     ; preds = %69, %54
  %74 = phi slice [ bitcast (i257 0 to slice), %54 ], [ %71, %69 ]
  %75 = phi i257 [ 0, %54 ], [ 1, %69 ]
  %76 = phi slice [ %67, %54 ], [ %72, %69 ]
  %77 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %76) #6
  %78 = extractvalue { slice, slice } %77, 1
  %79 = tail call { i257, slice } @llvm.tvm.ldu(slice %78, i257 1) #6
  %80 = extractvalue { i257, slice } %79, 0
  %81 = extractvalue { i257, slice } %79, 1
  %82 = icmp eq i257 %80, 0
  br i1 %82, label %87, label %83

; <label>:83:                                     ; preds = %73
  %84 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %81) #6
  %85 = extractvalue { slice, slice } %84, 0
  %86 = extractvalue { slice, slice } %84, 1
  br label %87

; <label>:87:                                     ; preds = %83, %73
  %88 = phi slice [ bitcast (i257 0 to slice), %73 ], [ %85, %83 ]
  %89 = phi i257 [ 0, %73 ], [ 1, %83 ]
  %90 = phi slice [ %81, %73 ], [ %86, %83 ]
  %91 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %90) #6
  %92 = extractvalue { slice, slice } %91, 1
  %93 = tail call { i257, slice } @llvm.tvm.ldu(slice %92, i257 1) #6
  %94 = extractvalue { i257, slice } %93, 0
  %95 = icmp eq i257 %94, 0
  br i1 %95, label %100, label %96

; <label>:96:                                     ; preds = %87
  %97 = extractvalue { i257, slice } %93, 1
  %98 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %97) #6
  %99 = extractvalue { slice, slice } %98, 0
  br label %100

; <label>:100:                                    ; preds = %96, %87
  %101 = phi slice [ bitcast (i257 0 to slice), %87 ], [ %99, %96 ]
  %102 = phi i257 [ 0, %87 ], [ 1, %96 ]
  %103 = icmp ne i257 %58, 0
  %104 = icmp uge i257 %58, %22
  %105 = and i1 %103, %104
  br i1 %105, label %112, label %106

; <label>:106:                                    ; preds = %100
  %107 = tail call i257 @llvm.tvm.now() #6
  %108 = mul i257 %107, 1000
  %109 = add i257 %108, 1800000
  %110 = icmp ugt i257 %109, %22
  %111 = select i1 %110, i257 %22, i257 0
  br i1 %110, label %113, label %112

; <label>:112:                                    ; preds = %106, %100
  tail call void @llvm.tvm.throw(i257 60) #6
  unreachable

; <label>:113:                                    ; preds = %106
  tail call void @llvm.tvm.accept() #6
  %114 = tail call builder @llvm.tvm.newc() #6
  %115 = tail call slice @llvm.tvm.ctos(cell %0) #6
  %116 = tail call { i257, slice } @llvm.tvm.ldu(slice %115, i257 2) #6
  %117 = extractvalue { i257, slice } %116, 0
  %118 = icmp eq i257 %117, 2
  br i1 %118, label %120, label %119

; <label>:119:                                    ; preds = %113
  tail call void @llvm.tvm.throw(i257 48) #6
  unreachable

; <label>:120:                                    ; preds = %113
  %121 = tail call builder @llvm.tvm.stu(i257 0, builder %114, i257 2) #6
  %122 = tail call cell @llvm.tvm.endc(builder %121) #6
  %123 = tail call slice @llvm.tvm.ctos(cell %122) #6
  %124 = extractvalue { i257, slice } %116, 1
  %125 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %124) #6
  %126 = extractvalue { slice, slice } %125, 0
  %127 = tail call builder @llvm.tvm.stu(i257 3, builder %114, i257 2) #6
  %128 = tail call builder @llvm.tvm.stslice(slice %123, builder %127) #6
  %129 = tail call builder @llvm.tvm.stslice(slice %126, builder %128) #6
  %130 = tail call builder @llvm.tvm.stu(i257 0, builder %129, i257 64) #6
  %131 = tail call builder @llvm.tvm.stu(i257 0, builder %130, i257 32) #6
  %132 = tail call builder @llvm.tvm.stu(i257 0, builder %131, i257 1) #6
  %133 = tail call builder @llvm.tvm.stu(i257 1, builder %132, i257 1) #6
  %134 = tail call builder @llvm.tvm.stu(i257 3905222791, builder %114, i257 32) #6
  %135 = tail call builder @llvm.tvm.stu(i257 0, builder %134, i257 32) #6
  %136 = tail call builder @llvm.tvm.stdict(cell %4, builder %135) #6
  %137 = tail call cell @llvm.tvm.endc(builder %136) #6
  %138 = tail call builder @llvm.tvm.stref(cell %137, builder %133) #6
  %139 = tail call cell @llvm.tvm.endc(builder %138) #6
  tail call void @llvm.tvm.sendrawmsg(cell %139, i257 0) #6
  %140 = icmp eq i257 %75, 0
  %141 = icmp eq i257 %89, 0
  %142 = icmp eq i257 %102, 0
  %143 = tail call builder @llvm.tvm.newc() #6
  %144 = tail call builder @llvm.tvm.stu(i257 0, builder %143, i257 1) #6
  %145 = tail call builder @llvm.tvm.stu(i257 %111, builder %144, i257 64) #6
  %146 = tail call builder @llvm.tvm.stu(i257 %62, builder %145, i257 32) #6
  %147 = tail call builder @llvm.tvm.stdict(cell %63, builder %146) #6
  br i1 %140, label %151, label %148

; <label>:148:                                    ; preds = %120
  %149 = tail call builder @llvm.tvm.stu(i257 1, builder %147, i257 1) #6
  %150 = tail call builder @llvm.tvm.stslice(slice %74, builder %149) #6
  br label %153

; <label>:151:                                    ; preds = %120
  %152 = tail call builder @llvm.tvm.stu(i257 0, builder %147, i257 1) #6
  br label %153

; <label>:153:                                    ; preds = %151, %148
  %154 = phi builder [ %150, %148 ], [ %152, %151 ]
  br i1 %141, label %158, label %155

; <label>:155:                                    ; preds = %153
  %156 = tail call builder @llvm.tvm.stu(i257 1, builder %143, i257 1) #6
  %157 = tail call builder @llvm.tvm.stslice(slice %88, builder %156) #6
  br label %158

; <label>:158:                                    ; preds = %155, %153
  %159 = phi builder [ %157, %155 ], [ %144, %153 ]
  br i1 %142, label %163, label %160

; <label>:160:                                    ; preds = %158
  %161 = tail call builder @llvm.tvm.stu(i257 1, builder %143, i257 1) #6
  %162 = tail call builder @llvm.tvm.stslice(slice %101, builder %161) #6
  br label %163

; <label>:163:                                    ; preds = %160, %158
  %164 = phi builder [ %162, %160 ], [ %144, %158 ]
  %165 = tail call cell @llvm.tvm.endc(builder %164) #6
  %166 = tail call builder @llvm.tvm.stref(cell %165, builder %159) #6
  %167 = tail call cell @llvm.tvm.endc(builder %166) #6
  %168 = tail call builder @llvm.tvm.stref(cell %167, builder %154) #6
  %169 = tail call cell @llvm.tvm.endc(builder %168) #6
  tail call void @llvm.tvm.set.persistent.data(cell %169) #6
  br label %_ZN3tvm12smart_switchILb0E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit

; <label>:170:                                    ; preds = %19
  %171 = tail call cell @llvm.tvm.get.persistent.data() #6
  %172 = tail call slice @llvm.tvm.ctos(cell %171) #6
  %173 = tail call { i257, slice } @llvm.tvm.ldu(slice %172, i257 1) #6
  %174 = extractvalue { i257, slice } %173, 0
  %175 = icmp eq i257 %174, 0
  br i1 %175, label %177, label %176

; <label>:176:                                    ; preds = %170
  tail call void @llvm.tvm.throw(i257 63) #6
  unreachable

; <label>:177:                                    ; preds = %170
  %178 = extractvalue { i257, slice } %173, 1
  %179 = tail call { i257, slice } @llvm.tvm.ldu(slice %178, i257 64) #6
  %180 = extractvalue { i257, slice } %179, 1
  %181 = extractvalue { i257, slice } %179, 0
  %182 = tail call { i257, slice } @llvm.tvm.ldu(slice %180, i257 32) #6
  %183 = extractvalue { i257, slice } %182, 1
  %184 = tail call { cell, slice } @llvm.tvm.lddict(slice %183) #6
  %185 = extractvalue { i257, slice } %182, 0
  %186 = extractvalue { cell, slice } %184, 0
  %187 = extractvalue { cell, slice } %184, 1
  %188 = tail call { i257, slice } @llvm.tvm.ldu(slice %187, i257 1) #6
  %189 = extractvalue { i257, slice } %188, 0
  %190 = extractvalue { i257, slice } %188, 1
  %191 = icmp eq i257 %189, 0
  br i1 %191, label %196, label %192

; <label>:192:                                    ; preds = %177
  %193 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %190) #6
  %194 = extractvalue { slice, slice } %193, 0
  %195 = extractvalue { slice, slice } %193, 1
  br label %196

; <label>:196:                                    ; preds = %192, %177
  %197 = phi slice [ bitcast (i257 0 to slice), %177 ], [ %194, %192 ]
  %198 = phi i257 [ 0, %177 ], [ 1, %192 ]
  %199 = phi slice [ %190, %177 ], [ %195, %192 ]
  %200 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %199) #6
  %201 = extractvalue { slice, slice } %200, 1
  %202 = tail call { i257, slice } @llvm.tvm.ldu(slice %201, i257 1) #6
  %203 = extractvalue { i257, slice } %202, 0
  %204 = extractvalue { i257, slice } %202, 1
  %205 = icmp eq i257 %203, 0
  br i1 %205, label %210, label %206

; <label>:206:                                    ; preds = %196
  %207 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %204) #6
  %208 = extractvalue { slice, slice } %207, 0
  %209 = extractvalue { slice, slice } %207, 1
  br label %210

; <label>:210:                                    ; preds = %206, %196
  %211 = phi slice [ bitcast (i257 0 to slice), %196 ], [ %208, %206 ]
  %212 = phi i257 [ 0, %196 ], [ 1, %206 ]
  %213 = phi slice [ %204, %196 ], [ %209, %206 ]
  %214 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %213) #6
  %215 = extractvalue { slice, slice } %214, 1
  %216 = tail call { i257, slice } @llvm.tvm.ldu(slice %215, i257 1) #6
  %217 = extractvalue { i257, slice } %216, 0
  %218 = icmp eq i257 %217, 0
  br i1 %218, label %223, label %219

; <label>:219:                                    ; preds = %210
  %220 = extractvalue { i257, slice } %216, 1
  %221 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %220) #6
  %222 = extractvalue { slice, slice } %221, 0
  br label %223

; <label>:223:                                    ; preds = %219, %210
  %224 = phi slice [ bitcast (i257 0 to slice), %210 ], [ %222, %219 ]
  %225 = phi i257 [ 0, %210 ], [ 1, %219 ]
  %226 = icmp ne i257 %181, 0
  %227 = icmp uge i257 %181, %22
  %228 = and i1 %226, %227
  br i1 %228, label %235, label %229

; <label>:229:                                    ; preds = %223
  %230 = tail call i257 @llvm.tvm.now() #6
  %231 = mul i257 %230, 1000
  %232 = add i257 %231, 1800000
  %233 = icmp ugt i257 %232, %22
  %234 = select i1 %233, i257 %22, i257 0
  br i1 %233, label %236, label %235

; <label>:235:                                    ; preds = %229, %223
  tail call void @llvm.tvm.throw(i257 60) #6
  unreachable

; <label>:236:                                    ; preds = %229
  tail call void @llvm.tvm.accept() #6
  %237 = tail call { cell, i257 } @llvm.tvm.dictugetref(i257 %185, cell %186, i257 32) #6
  %238 = extractvalue { cell, i257 } %237, 1
  %239 = icmp eq i257 %238, 0
  %240 = add i257 %185, 1
  %241 = and i257 %240, 2147483647
  %242 = select i1 %239, i257 %185, i257 %241
  tail call void @llvm.tvm.setglobal(i257 4, i257 %242) #6
  %243 = tail call builder @llvm.tvm.newc() #6, !noalias !2
  %244 = tail call builder @llvm.tvm.stu(i257 1775149307253347665615585800530890380756025176765988145664, builder %243, i257 192) #6
  %245 = tail call cell @llvm.tvm.endc(builder %244) #6
  %246 = tail call builder @llvm.tvm.stu(i257 0, builder %243, i257 2) #6
  %247 = tail call cell @llvm.tvm.endc(builder %246) #6
  %248 = tail call slice @llvm.tvm.ctos(cell %247) #6
  %249 = tail call builder @llvm.tvm.stu(i257 0, builder %243, i257 1) #6
  %250 = tail call builder @llvm.tvm.stu(i257 1, builder %249, i257 1) #6
  %251 = tail call builder @llvm.tvm.stu(i257 1, builder %250, i257 1) #6
  %252 = tail call builder @llvm.tvm.stu(i257 0, builder %251, i257 1) #6
  %253 = tail call builder @llvm.tvm.stslice(slice %248, builder %252) #6
  %254 = tail call builder @llvm.tvm.stslice(slice %11, builder %253) #6
  %255 = tail call builder @llvm.tvm.stvaruint16(builder %254, i257 10000000) #6
  %256 = tail call builder @llvm.tvm.stdict(cell %4, builder %255) #6
  %257 = tail call builder @llvm.tvm.stvaruint16(builder %256, i257 0) #6
  %258 = tail call builder @llvm.tvm.stvaruint16(builder %257, i257 0) #6
  %259 = tail call builder @llvm.tvm.stu(i257 0, builder %258, i257 64) #6
  %260 = tail call builder @llvm.tvm.stu(i257 0, builder %259, i257 32) #6
  %261 = tail call builder @llvm.tvm.stu(i257 0, builder %260, i257 1) #6
  %262 = tail call builder @llvm.tvm.stu(i257 1, builder %261, i257 1) #6
  %263 = tail call builder @llvm.tvm.stu(i257 1412243880, builder %243, i257 32) #6
  %264 = tail call builder @llvm.tvm.stref(cell %245, builder %263) #6
  %265 = tail call cell @llvm.tvm.endc(builder %264) #6
  %266 = tail call builder @llvm.tvm.stref(cell %265, builder %262) #6
  %267 = tail call cell @llvm.tvm.endc(builder %266) #6
  tail call void @llvm.tvm.sendrawmsg(cell %267, i257 3) #6, !noalias !2
  %268 = tail call i257 @llvm.tvm.cast.from.slice(slice %11) #6
  tail call void @llvm.tvm.setglobal(i257 2, i257 %268) #6, !noalias !2
  %269 = tail call i257 @llvm.tvm.getglobal(i257 2) #6
  %270 = tail call slice @llvm.tvm.cast.to.slice(i257 %269) #6
  %271 = tail call { i257, slice } @llvm.tvm.ldu(slice %270, i257 2) #6
  %272 = extractvalue { i257, slice } %271, 0
  switch i257 %272, label %283 [
    i257 2, label %273
    i257 3, label %282
  ]

; <label>:273:                                    ; preds = %236
  %274 = extractvalue { i257, slice } %271, 1
  %275 = tail call { i257, slice } @llvm.tvm.ldu(slice %274, i257 1) #6
  %276 = extractvalue { i257, slice } %275, 0
  %277 = extractvalue { i257, slice } %275, 1
  %278 = icmp eq i257 %276, 0
  br i1 %278, label %284, label %279

; <label>:279:                                    ; preds = %273
  %280 = tail call { i257, slice } @llvm.tvm.ldvaruint32(slice %277) #6
  %281 = extractvalue { i257, slice } %280, 1
  br label %284

; <label>:282:                                    ; preds = %236
  tail call void @llvm.tvm.throw(i257 99) #6
  unreachable

; <label>:283:                                    ; preds = %236
  tail call void @llvm.tvm.throw(i257 52) #6
  unreachable

; <label>:284:                                    ; preds = %279, %273
  %285 = phi slice [ %277, %273 ], [ %281, %279 ]
  %286 = tail call { i257, slice } @llvm.tvm.ldi(slice %285, i257 8) #6
  %287 = tail call slice @llvm.tvm.ctos(cell %0) #6
  %288 = tail call { i257, slice } @llvm.tvm.ldu(slice %287, i257 2) #6
  %289 = extractvalue { i257, slice } %288, 0
  %290 = icmp eq i257 %289, 2
  br i1 %290, label %292, label %291

; <label>:291:                                    ; preds = %284
  tail call void @llvm.tvm.throw(i257 48) #6
  unreachable

; <label>:292:                                    ; preds = %284
  %293 = extractvalue { i257, slice } %286, 1
  %294 = tail call { i257, slice } @llvm.tvm.ldu(slice %293, i257 256) #6
  %295 = extractvalue { i257, slice } %294, 0
  %296 = extractvalue { i257, slice } %286, 0
  %297 = extractvalue { i257, slice } %288, 1
  %298 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %297) #6
  %299 = extractvalue { slice, slice } %298, 0
  %300 = tail call builder @llvm.tvm.newc() #6
  %301 = tail call cell @llvm.tvm.endc(builder undef) #6
  %302 = tail call builder @llvm.tvm.sti(i257 undef, builder %300, i257 257) #6
  %303 = tail call builder @llvm.tvm.sti(i257 undef, builder %302, i257 257) #6
  %304 = tail call builder @llvm.tvm.stref(cell undef, builder %303) #6
  %305 = tail call builder @llvm.tvm.stref(cell %301, builder %304) #6
  %306 = tail call cell @llvm.tvm.endc(builder %305) #6
  %307 = tail call builder @llvm.tvm.stslice(slice undef, builder %300) #6
  %308 = tail call cell @llvm.tvm.endc(builder %307) #6
  %309 = tail call builder @llvm.tvm.stref(cell undef, builder %302) #6
  %310 = tail call builder @llvm.tvm.sti(i257 undef, builder %309, i257 257) #6
  %311 = tail call builder @llvm.tvm.stref(cell %301, builder %310) #6
  %312 = tail call builder @llvm.tvm.stref(cell %308, builder %311) #6
  %313 = tail call builder @llvm.tvm.sti(i257 undef, builder %312, i257 257) #6
  %314 = tail call builder @llvm.tvm.stref(cell %306, builder %313) #6
  %315 = tail call cell @llvm.tvm.endc(builder %314) #6
  %316 = tail call builder @llvm.tvm.sti(i257 undef, builder %310, i257 257) #6
  %317 = tail call builder @llvm.tvm.stref(cell undef, builder %316) #6
  %318 = tail call builder @llvm.tvm.stref(cell undef, builder %317) #6
  %319 = tail call builder @llvm.tvm.stref(cell %315, builder %318) #6
  %320 = tail call cell @llvm.tvm.endc(builder %319) #6
  %321 = tail call builder @llvm.tvm.sti(i257 undef, builder %303, i257 257) #6
  %322 = tail call builder @llvm.tvm.stref(cell %320, builder %321) #6
  %323 = tail call cell @llvm.tvm.endc(builder %322) #6
  %324 = tail call builder @llvm.tvm.sti(i257 undef, builder %304, i257 257) #6
  %325 = tail call builder @llvm.tvm.stref(cell %323, builder %324) #6
  %326 = tail call cell @llvm.tvm.endc(builder %325) #6
  %327 = tail call builder @llvm.tvm.stslice(slice %299, builder %300) #6
  %328 = tail call cell @llvm.tvm.endc(builder %327) #6
  %329 = tail call builder @llvm.tvm.stu(i257 94113135, builder %300, i257 32) #6
  %330 = tail call builder @llvm.tvm.sti(i257 %296, builder %329, i257 8) #6
  %331 = tail call builder @llvm.tvm.stu(i257 %295, builder %330, i257 256) #6
  %332 = tail call builder @llvm.tvm.stref(cell %328, builder %331) #6
  %333 = tail call builder @llvm.tvm.stu(i257 0, builder %332, i257 6) #6
  %334 = tail call builder @llvm.tvm.sti(i257 undef, builder %333, i257 257) #6
  %335 = tail call builder @llvm.tvm.stref(cell %308, builder %334) #6
  %336 = tail call builder @llvm.tvm.sti(i257 undef, builder %335, i257 257) #6
  %337 = tail call builder @llvm.tvm.stref(cell %326, builder %336) #6
  %338 = tail call cell @llvm.tvm.endc(builder %337) #6
  %339 = tail call cell @llvm.tvm.dictusetref(cell %338, i257 %242, cell %186, i257 32) #6
  %340 = icmp eq i257 %198, 0
  %341 = icmp eq i257 %212, 0
  %342 = icmp eq i257 %225, 0
  %343 = tail call builder @llvm.tvm.newc() #6
  %344 = tail call builder @llvm.tvm.stu(i257 0, builder %343, i257 1) #6
  %345 = tail call builder @llvm.tvm.stu(i257 %234, builder %344, i257 64) #6
  %346 = tail call builder @llvm.tvm.stu(i257 %242, builder %345, i257 32) #6
  %347 = tail call builder @llvm.tvm.stdict(cell %339, builder %346) #6
  br i1 %340, label %351, label %348

; <label>:348:                                    ; preds = %292
  %349 = tail call builder @llvm.tvm.stu(i257 1, builder %347, i257 1) #6
  %350 = tail call builder @llvm.tvm.stslice(slice %197, builder %349) #6
  br label %353

; <label>:351:                                    ; preds = %292
  %352 = tail call builder @llvm.tvm.stu(i257 0, builder %347, i257 1) #6
  br label %353

; <label>:353:                                    ; preds = %351, %348
  %354 = phi builder [ %350, %348 ], [ %352, %351 ]
  br i1 %341, label %358, label %355

; <label>:355:                                    ; preds = %353
  %356 = tail call builder @llvm.tvm.stu(i257 1, builder %343, i257 1) #6
  %357 = tail call builder @llvm.tvm.stslice(slice %211, builder %356) #6
  br label %358

; <label>:358:                                    ; preds = %355, %353
  %359 = phi builder [ %357, %355 ], [ %344, %353 ]
  br i1 %342, label %363, label %360

; <label>:360:                                    ; preds = %358
  %361 = tail call builder @llvm.tvm.stu(i257 1, builder %343, i257 1) #6
  %362 = tail call builder @llvm.tvm.stslice(slice %224, builder %361) #6
  br label %363

; <label>:363:                                    ; preds = %360, %358
  %364 = phi builder [ %362, %360 ], [ %344, %358 ]
  %365 = tail call cell @llvm.tvm.endc(builder %364) #6
  %366 = tail call builder @llvm.tvm.stref(cell %365, builder %359) #6
  %367 = tail call cell @llvm.tvm.endc(builder %366) #6
  %368 = tail call builder @llvm.tvm.stref(cell %367, builder %354) #6
  %369 = tail call cell @llvm.tvm.endc(builder %368) #6
  tail call void @llvm.tvm.set.persistent.data(cell %369) #6
  br label %_ZN3tvm12smart_switchILb0E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit

; <label>:370:                                    ; preds = %19
  %371 = tail call cell @llvm.tvm.get.persistent.data() #6
  %372 = tail call slice @llvm.tvm.ctos(cell %371) #6
  %373 = tail call { i257, slice } @llvm.tvm.ldu(slice %372, i257 1) #6
  %374 = extractvalue { i257, slice } %373, 0
  %375 = icmp eq i257 %374, 0
  br i1 %375, label %377, label %376

; <label>:376:                                    ; preds = %370
  tail call void @llvm.tvm.throw(i257 63) #6
  unreachable

; <label>:377:                                    ; preds = %370
  %378 = extractvalue { i257, slice } %373, 1
  %379 = tail call { i257, slice } @llvm.tvm.ldu(slice %378, i257 64) #6
  %380 = extractvalue { i257, slice } %379, 1
  %381 = extractvalue { i257, slice } %379, 0
  %382 = tail call { i257, slice } @llvm.tvm.ldu(slice %380, i257 32) #6
  %383 = extractvalue { i257, slice } %382, 1
  %384 = tail call { cell, slice } @llvm.tvm.lddict(slice %383) #6
  %385 = extractvalue { i257, slice } %382, 0
  %386 = extractvalue { cell, slice } %384, 0
  %387 = extractvalue { cell, slice } %384, 1
  %388 = tail call { i257, slice } @llvm.tvm.ldu(slice %387, i257 1) #6
  %389 = extractvalue { i257, slice } %388, 0
  %390 = extractvalue { i257, slice } %388, 1
  %391 = icmp eq i257 %389, 0
  br i1 %391, label %396, label %392

; <label>:392:                                    ; preds = %377
  %393 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %390) #6
  %394 = extractvalue { slice, slice } %393, 0
  %395 = extractvalue { slice, slice } %393, 1
  br label %396

; <label>:396:                                    ; preds = %392, %377
  %397 = phi slice [ bitcast (i257 0 to slice), %377 ], [ %394, %392 ]
  %398 = phi i257 [ 0, %377 ], [ 1, %392 ]
  %399 = phi slice [ %390, %377 ], [ %395, %392 ]
  %400 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %399) #6
  %401 = extractvalue { slice, slice } %400, 1
  %402 = tail call { i257, slice } @llvm.tvm.ldu(slice %401, i257 1) #6
  %403 = extractvalue { i257, slice } %402, 0
  %404 = extractvalue { i257, slice } %402, 1
  %405 = icmp eq i257 %403, 0
  br i1 %405, label %410, label %406

; <label>:406:                                    ; preds = %396
  %407 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %404) #6
  %408 = extractvalue { slice, slice } %407, 0
  %409 = extractvalue { slice, slice } %407, 1
  br label %410

; <label>:410:                                    ; preds = %406, %396
  %411 = phi slice [ bitcast (i257 0 to slice), %396 ], [ %408, %406 ]
  %412 = phi i257 [ 0, %396 ], [ 1, %406 ]
  %413 = phi slice [ %404, %396 ], [ %409, %406 ]
  %414 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %413) #6
  %415 = extractvalue { slice, slice } %414, 1
  %416 = tail call { i257, slice } @llvm.tvm.ldu(slice %415, i257 1) #6
  %417 = extractvalue { i257, slice } %416, 0
  %418 = icmp eq i257 %417, 0
  br i1 %418, label %423, label %419

; <label>:419:                                    ; preds = %410
  %420 = extractvalue { i257, slice } %416, 1
  %421 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %420) #6
  %422 = extractvalue { slice, slice } %421, 0
  br label %423

; <label>:423:                                    ; preds = %419, %410
  %424 = phi slice [ bitcast (i257 0 to slice), %410 ], [ %422, %419 ]
  %425 = phi i257 [ 0, %410 ], [ 1, %419 ]
  %426 = icmp ne i257 %381, 0
  %427 = icmp uge i257 %381, %22
  %428 = and i1 %426, %427
  br i1 %428, label %434, label %429

; <label>:429:                                    ; preds = %423
  %430 = tail call i257 @llvm.tvm.now() #6
  %431 = mul i257 %430, 1000
  %432 = add i257 %431, 1800000
  %433 = icmp ugt i257 %432, %22
  br i1 %433, label %435, label %434

; <label>:434:                                    ; preds = %429, %423
  tail call void @llvm.tvm.throw(i257 60) #6
  unreachable

; <label>:435:                                    ; preds = %429
  tail call void @llvm.tvm.accept() #6
  %436 = icmp eq i257 %398, 0
  %437 = icmp eq i257 %412, 0
  %438 = icmp eq i257 %425, 0
  %439 = tail call builder @llvm.tvm.newc() #6
  %440 = tail call builder @llvm.tvm.stu(i257 0, builder %439, i257 1) #6
  %441 = tail call builder @llvm.tvm.stu(i257 %22, builder %440, i257 64) #6
  %442 = tail call builder @llvm.tvm.stu(i257 %385, builder %441, i257 32) #6
  %443 = tail call builder @llvm.tvm.stdict(cell %386, builder %442) #6
  br i1 %436, label %447, label %444

; <label>:444:                                    ; preds = %435
  %445 = tail call builder @llvm.tvm.stu(i257 1, builder %443, i257 1) #6
  %446 = tail call builder @llvm.tvm.stslice(slice %397, builder %445) #6
  br label %449

; <label>:447:                                    ; preds = %435
  %448 = tail call builder @llvm.tvm.stu(i257 0, builder %443, i257 1) #6
  br label %449

; <label>:449:                                    ; preds = %447, %444
  %450 = phi builder [ %446, %444 ], [ %448, %447 ]
  br i1 %437, label %454, label %451

; <label>:451:                                    ; preds = %449
  %452 = tail call builder @llvm.tvm.stu(i257 1, builder %439, i257 1) #6
  %453 = tail call builder @llvm.tvm.stslice(slice %411, builder %452) #6
  br label %454

; <label>:454:                                    ; preds = %451, %449
  %455 = phi builder [ %453, %451 ], [ %440, %449 ]
  br i1 %438, label %459, label %456

; <label>:456:                                    ; preds = %454
  %457 = tail call builder @llvm.tvm.stu(i257 1, builder %439, i257 1) #6
  %458 = tail call builder @llvm.tvm.stslice(slice %424, builder %457) #6
  br label %459

; <label>:459:                                    ; preds = %456, %454
  %460 = phi builder [ %458, %456 ], [ %440, %454 ]
  %461 = tail call cell @llvm.tvm.endc(builder %460) #6
  %462 = tail call builder @llvm.tvm.stref(cell %461, builder %455) #6
  %463 = tail call cell @llvm.tvm.endc(builder %462) #6
  %464 = tail call builder @llvm.tvm.stref(cell %463, builder %450) #6
  %465 = tail call cell @llvm.tvm.endc(builder %464) #6
  tail call void @llvm.tvm.set.persistent.data(cell %465) #6
  br label %_ZN3tvm12smart_switchILb0E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit

; <label>:466:                                    ; preds = %19
  %467 = tail call cell @llvm.tvm.get.persistent.data() #6
  %468 = tail call slice @llvm.tvm.ctos(cell %467) #6
  %469 = tail call { i257, slice } @llvm.tvm.ldu(slice %468, i257 1) #6
  %470 = extractvalue { i257, slice } %469, 0
  %471 = icmp eq i257 %470, 0
  br i1 %471, label %473, label %472

; <label>:472:                                    ; preds = %466
  tail call void @llvm.tvm.throw(i257 63) #6
  unreachable

; <label>:473:                                    ; preds = %466
  %474 = extractvalue { i257, slice } %469, 1
  %475 = tail call { i257, slice } @llvm.tvm.ldu(slice %474, i257 64) #6
  %476 = extractvalue { i257, slice } %475, 1
  %477 = extractvalue { i257, slice } %475, 0
  %478 = tail call { i257, slice } @llvm.tvm.ldu(slice %476, i257 32) #6
  %479 = extractvalue { i257, slice } %478, 1
  %480 = tail call { cell, slice } @llvm.tvm.lddict(slice %479) #6
  %481 = extractvalue { i257, slice } %478, 0
  %482 = extractvalue { cell, slice } %480, 0
  %483 = extractvalue { cell, slice } %480, 1
  %484 = tail call { i257, slice } @llvm.tvm.ldu(slice %483, i257 1) #6
  %485 = extractvalue { i257, slice } %484, 0
  %486 = extractvalue { i257, slice } %484, 1
  %487 = icmp eq i257 %485, 0
  br i1 %487, label %492, label %488

; <label>:488:                                    ; preds = %473
  %489 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %486) #6
  %490 = extractvalue { slice, slice } %489, 0
  %491 = extractvalue { slice, slice } %489, 1
  br label %492

; <label>:492:                                    ; preds = %488, %473
  %493 = phi slice [ bitcast (i257 0 to slice), %473 ], [ %490, %488 ]
  %494 = phi i257 [ 0, %473 ], [ 1, %488 ]
  %495 = phi slice [ %486, %473 ], [ %491, %488 ]
  %496 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %495) #6
  %497 = extractvalue { slice, slice } %496, 1
  %498 = tail call { i257, slice } @llvm.tvm.ldu(slice %497, i257 1) #6
  %499 = extractvalue { i257, slice } %498, 0
  %500 = extractvalue { i257, slice } %498, 1
  %501 = icmp eq i257 %499, 0
  br i1 %501, label %506, label %502

; <label>:502:                                    ; preds = %492
  %503 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %500) #6
  %504 = extractvalue { slice, slice } %503, 0
  %505 = extractvalue { slice, slice } %503, 1
  br label %506

; <label>:506:                                    ; preds = %502, %492
  %507 = phi slice [ bitcast (i257 0 to slice), %492 ], [ %504, %502 ]
  %508 = phi i257 [ 0, %492 ], [ 1, %502 ]
  %509 = phi slice [ %500, %492 ], [ %505, %502 ]
  %510 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %509) #6
  %511 = extractvalue { slice, slice } %510, 1
  %512 = tail call { i257, slice } @llvm.tvm.ldu(slice %511, i257 1) #6
  %513 = extractvalue { i257, slice } %512, 0
  %514 = icmp eq i257 %513, 0
  br i1 %514, label %519, label %515

; <label>:515:                                    ; preds = %506
  %516 = extractvalue { i257, slice } %512, 1
  %517 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %516) #6
  %518 = extractvalue { slice, slice } %517, 0
  br label %519

; <label>:519:                                    ; preds = %515, %506
  %520 = phi slice [ bitcast (i257 0 to slice), %506 ], [ %518, %515 ]
  %521 = phi i257 [ 0, %506 ], [ 1, %515 ]
  %522 = icmp ne i257 %477, 0
  %523 = icmp uge i257 %477, %22
  %524 = and i1 %522, %523
  br i1 %524, label %531, label %525

; <label>:525:                                    ; preds = %519
  %526 = tail call i257 @llvm.tvm.now() #6
  %527 = mul i257 %526, 1000
  %528 = add i257 %527, 1800000
  %529 = icmp ugt i257 %528, %22
  %530 = select i1 %529, i257 %22, i257 0
  br i1 %529, label %532, label %531

; <label>:531:                                    ; preds = %525, %519
  tail call void @llvm.tvm.throw(i257 60) #6
  unreachable

; <label>:532:                                    ; preds = %525
  tail call void @llvm.tvm.accept() #6
  %533 = tail call builder @llvm.tvm.newc() #6
  %534 = tail call slice @llvm.tvm.ctos(cell %0) #6
  %535 = tail call { i257, slice } @llvm.tvm.ldu(slice %534, i257 2) #6
  %536 = extractvalue { i257, slice } %535, 0
  %537 = icmp eq i257 %536, 2
  br i1 %537, label %539, label %538

; <label>:538:                                    ; preds = %532
  tail call void @llvm.tvm.throw(i257 48) #6
  unreachable

; <label>:539:                                    ; preds = %532
  %540 = tail call builder @llvm.tvm.stu(i257 0, builder %533, i257 2) #6
  %541 = tail call cell @llvm.tvm.endc(builder %540) #6
  %542 = tail call slice @llvm.tvm.ctos(cell %541) #6
  %543 = tail call builder @llvm.tvm.stu(i257 28346234202099125654094059144414768279808, builder %533, i257 136) #6
  %544 = tail call cell @llvm.tvm.endc(builder %543) #6
  %545 = extractvalue { i257, slice } %535, 1
  %546 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %545) #6
  %547 = extractvalue { slice, slice } %546, 0
  %548 = tail call builder @llvm.tvm.stu(i257 3, builder %533, i257 2) #6
  %549 = tail call builder @llvm.tvm.stslice(slice %542, builder %548) #6
  %550 = tail call builder @llvm.tvm.stslice(slice %547, builder %549) #6
  %551 = tail call builder @llvm.tvm.stu(i257 0, builder %550, i257 64) #6
  %552 = tail call builder @llvm.tvm.stu(i257 0, builder %551, i257 32) #6
  %553 = tail call builder @llvm.tvm.stu(i257 0, builder %552, i257 1) #6
  %554 = tail call builder @llvm.tvm.stu(i257 1, builder %553, i257 1) #6
  %555 = tail call builder @llvm.tvm.stu(i257 3040958319, builder %533, i257 32) #6
  %556 = tail call builder @llvm.tvm.stref(cell %544, builder %555) #6
  %557 = tail call builder @llvm.tvm.stu(i257 257, builder %556, i257 24) #6
  %558 = tail call cell @llvm.tvm.endc(builder %557) #6
  %559 = tail call builder @llvm.tvm.stref(cell %558, builder %554) #6
  %560 = tail call cell @llvm.tvm.endc(builder %559) #6
  tail call void @llvm.tvm.sendrawmsg(cell %560, i257 0) #6
  %561 = icmp eq i257 %494, 0
  %562 = icmp eq i257 %508, 0
  %563 = icmp eq i257 %521, 0
  %564 = tail call builder @llvm.tvm.newc() #6
  %565 = tail call builder @llvm.tvm.stu(i257 0, builder %564, i257 1) #6
  %566 = tail call builder @llvm.tvm.stu(i257 %530, builder %565, i257 64) #6
  %567 = tail call builder @llvm.tvm.stu(i257 %481, builder %566, i257 32) #6
  %568 = tail call builder @llvm.tvm.stdict(cell %482, builder %567) #6
  br i1 %561, label %572, label %569

; <label>:569:                                    ; preds = %539
  %570 = tail call builder @llvm.tvm.stu(i257 1, builder %568, i257 1) #6
  %571 = tail call builder @llvm.tvm.stslice(slice %493, builder %570) #6
  br label %574

; <label>:572:                                    ; preds = %539
  %573 = tail call builder @llvm.tvm.stu(i257 0, builder %568, i257 1) #6
  br label %574

; <label>:574:                                    ; preds = %572, %569
  %575 = phi builder [ %571, %569 ], [ %573, %572 ]
  br i1 %562, label %579, label %576

; <label>:576:                                    ; preds = %574
  %577 = tail call builder @llvm.tvm.stu(i257 1, builder %564, i257 1) #6
  %578 = tail call builder @llvm.tvm.stslice(slice %507, builder %577) #6
  br label %579

; <label>:579:                                    ; preds = %576, %574
  %580 = phi builder [ %578, %576 ], [ %565, %574 ]
  br i1 %563, label %584, label %581

; <label>:581:                                    ; preds = %579
  %582 = tail call builder @llvm.tvm.stu(i257 1, builder %564, i257 1) #6
  %583 = tail call builder @llvm.tvm.stslice(slice %520, builder %582) #6
  br label %584

; <label>:584:                                    ; preds = %581, %579
  %585 = phi builder [ %583, %581 ], [ %565, %579 ]
  %586 = tail call cell @llvm.tvm.endc(builder %585) #6
  %587 = tail call builder @llvm.tvm.stref(cell %586, builder %580) #6
  %588 = tail call cell @llvm.tvm.endc(builder %587) #6
  %589 = tail call builder @llvm.tvm.stref(cell %588, builder %575) #6
  %590 = tail call cell @llvm.tvm.endc(builder %589) #6
  tail call void @llvm.tvm.set.persistent.data(cell %590) #6
  br label %_ZN3tvm12smart_switchILb0E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit

; <label>:591:                                    ; preds = %19
  %592 = tail call cell @llvm.tvm.get.persistent.data() #6
  %593 = tail call slice @llvm.tvm.ctos(cell %592) #6
  %594 = tail call { i257, slice } @llvm.tvm.ldu(slice %593, i257 1) #6
  %595 = extractvalue { i257, slice } %594, 0
  %596 = icmp eq i257 %595, 0
  br i1 %596, label %598, label %597

; <label>:597:                                    ; preds = %591
  tail call void @llvm.tvm.throw(i257 63) #6
  unreachable

; <label>:598:                                    ; preds = %591
  %599 = extractvalue { i257, slice } %594, 1
  %600 = tail call { i257, slice } @llvm.tvm.ldu(slice %599, i257 64) #6
  %601 = extractvalue { i257, slice } %600, 1
  %602 = extractvalue { i257, slice } %600, 0
  %603 = tail call { i257, slice } @llvm.tvm.ldu(slice %601, i257 32) #6
  %604 = extractvalue { i257, slice } %603, 1
  %605 = tail call { cell, slice } @llvm.tvm.lddict(slice %604) #6
  %606 = extractvalue { i257, slice } %603, 0
  %607 = extractvalue { cell, slice } %605, 0
  %608 = extractvalue { cell, slice } %605, 1
  %609 = tail call { i257, slice } @llvm.tvm.ldu(slice %608, i257 1) #6
  %610 = extractvalue { i257, slice } %609, 0
  %611 = extractvalue { i257, slice } %609, 1
  %612 = icmp eq i257 %610, 0
  br i1 %612, label %617, label %613

; <label>:613:                                    ; preds = %598
  %614 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %611) #6
  %615 = extractvalue { slice, slice } %614, 0
  %616 = extractvalue { slice, slice } %614, 1
  br label %617

; <label>:617:                                    ; preds = %613, %598
  %618 = phi slice [ bitcast (i257 0 to slice), %598 ], [ %615, %613 ]
  %619 = phi i257 [ 0, %598 ], [ 1, %613 ]
  %620 = phi slice [ %611, %598 ], [ %616, %613 ]
  %621 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %620) #6
  %622 = extractvalue { slice, slice } %621, 1
  %623 = tail call { i257, slice } @llvm.tvm.ldu(slice %622, i257 1) #6
  %624 = extractvalue { i257, slice } %623, 0
  %625 = extractvalue { i257, slice } %623, 1
  %626 = icmp eq i257 %624, 0
  br i1 %626, label %631, label %627

; <label>:627:                                    ; preds = %617
  %628 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %625) #6
  %629 = extractvalue { slice, slice } %628, 0
  %630 = extractvalue { slice, slice } %628, 1
  br label %631

; <label>:631:                                    ; preds = %627, %617
  %632 = phi slice [ bitcast (i257 0 to slice), %617 ], [ %629, %627 ]
  %633 = phi i257 [ 0, %617 ], [ 1, %627 ]
  %634 = phi slice [ %625, %617 ], [ %630, %627 ]
  %635 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %634) #6
  %636 = extractvalue { slice, slice } %635, 1
  %637 = tail call { i257, slice } @llvm.tvm.ldu(slice %636, i257 1) #6
  %638 = extractvalue { i257, slice } %637, 0
  %639 = icmp eq i257 %638, 0
  br i1 %639, label %644, label %640

; <label>:640:                                    ; preds = %631
  %641 = extractvalue { i257, slice } %637, 1
  %642 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %641) #6
  %643 = extractvalue { slice, slice } %642, 0
  br label %644

; <label>:644:                                    ; preds = %640, %631
  %645 = phi slice [ bitcast (i257 0 to slice), %631 ], [ %643, %640 ]
  %646 = phi i257 [ 0, %631 ], [ 1, %640 ]
  %647 = icmp ne i257 %602, 0
  %648 = icmp uge i257 %602, %22
  %649 = and i1 %647, %648
  br i1 %649, label %656, label %650

; <label>:650:                                    ; preds = %644
  %651 = tail call i257 @llvm.tvm.now() #6
  %652 = mul i257 %651, 1000
  %653 = add i257 %652, 1800000
  %654 = icmp ugt i257 %653, %22
  %655 = select i1 %654, i257 %22, i257 0
  br i1 %654, label %657, label %656

; <label>:656:                                    ; preds = %650, %644
  tail call void @llvm.tvm.throw(i257 60) #6
  unreachable

; <label>:657:                                    ; preds = %650
  tail call void @llvm.tvm.accept() #6
  %658 = tail call builder @llvm.tvm.newc() #6
  %659 = tail call slice @llvm.tvm.ctos(cell %0) #6
  %660 = tail call { i257, slice } @llvm.tvm.ldu(slice %659, i257 2) #6
  %661 = extractvalue { i257, slice } %660, 0
  %662 = icmp eq i257 %661, 2
  br i1 %662, label %664, label %663

; <label>:663:                                    ; preds = %657
  tail call void @llvm.tvm.throw(i257 48) #6
  unreachable

; <label>:664:                                    ; preds = %657
  %665 = tail call builder @llvm.tvm.stu(i257 0, builder %658, i257 8) #6
  %666 = tail call cell @llvm.tvm.endc(builder %665) #6
  %667 = tail call builder @llvm.tvm.stu(i257 0, builder %658, i257 2) #6
  %668 = tail call cell @llvm.tvm.endc(builder %667) #6
  %669 = tail call slice @llvm.tvm.ctos(cell %668) #6
  %670 = tail call builder @llvm.tvm.stu(i257 2, builder %658, i257 2) #6
  %671 = tail call builder @llvm.tvm.stu(i257 0, builder %670, i257 1) #6
  %672 = tail call builder @llvm.tvm.sti(i257 0, builder %671, i257 8) #6
  %673 = tail call builder @llvm.tvm.stu(i257 0, builder %672, i257 256) #6
  %674 = tail call cell @llvm.tvm.endc(builder %673) #6
  %675 = tail call slice @llvm.tvm.ctos(cell %674) #6
  %676 = extractvalue { i257, slice } %660, 1
  %677 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %676) #6
  %678 = extractvalue { slice, slice } %677, 0
  %679 = tail call builder @llvm.tvm.stu(i257 3, builder %658, i257 2) #6
  %680 = tail call builder @llvm.tvm.stslice(slice %669, builder %679) #6
  %681 = tail call builder @llvm.tvm.stslice(slice %678, builder %680) #6
  %682 = tail call builder @llvm.tvm.stu(i257 0, builder %681, i257 64) #6
  %683 = tail call builder @llvm.tvm.stu(i257 0, builder %682, i257 32) #6
  %684 = tail call builder @llvm.tvm.stu(i257 0, builder %683, i257 1) #6
  %685 = tail call builder @llvm.tvm.stu(i257 4260155611, builder %658, i257 32) #6
  %686 = tail call builder @llvm.tvm.stu(i257 0, builder %685, i257 8) #6
  %687 = tail call builder @llvm.tvm.stref(cell %666, builder %686) #6
  %688 = tail call builder @llvm.tvm.stref(cell %666, builder %687) #6
  %689 = tail call builder @llvm.tvm.stslice(slice %675, builder %688) #6
  %690 = tail call cell @llvm.tvm.endc(builder %689) #6
  %691 = tail call builder @llvm.tvm.stu(i257 1, builder %684, i257 1) #6
  %692 = tail call builder @llvm.tvm.stref(cell %690, builder %691) #6
  %693 = tail call cell @llvm.tvm.endc(builder %692) #6
  tail call void @llvm.tvm.sendrawmsg(cell %693, i257 0) #6
  %694 = icmp eq i257 %619, 0
  %695 = icmp eq i257 %633, 0
  %696 = icmp eq i257 %646, 0
  %697 = tail call builder @llvm.tvm.newc() #6
  %698 = tail call builder @llvm.tvm.stu(i257 0, builder %697, i257 1) #6
  %699 = tail call builder @llvm.tvm.stu(i257 %655, builder %698, i257 64) #6
  %700 = tail call builder @llvm.tvm.stu(i257 %606, builder %699, i257 32) #6
  %701 = tail call builder @llvm.tvm.stdict(cell %607, builder %700) #6
  br i1 %694, label %705, label %702

; <label>:702:                                    ; preds = %664
  %703 = tail call builder @llvm.tvm.stu(i257 1, builder %701, i257 1) #6
  %704 = tail call builder @llvm.tvm.stslice(slice %618, builder %703) #6
  br label %707

; <label>:705:                                    ; preds = %664
  %706 = tail call builder @llvm.tvm.stu(i257 0, builder %701, i257 1) #6
  br label %707

; <label>:707:                                    ; preds = %705, %702
  %708 = phi builder [ %704, %702 ], [ %706, %705 ]
  br i1 %695, label %712, label %709

; <label>:709:                                    ; preds = %707
  %710 = tail call builder @llvm.tvm.stu(i257 1, builder %697, i257 1) #6
  %711 = tail call builder @llvm.tvm.stslice(slice %632, builder %710) #6
  br label %712

; <label>:712:                                    ; preds = %709, %707
  %713 = phi builder [ %711, %709 ], [ %698, %707 ]
  br i1 %696, label %718, label %714

; <label>:714:                                    ; preds = %712
  %715 = tail call builder @llvm.tvm.stu(i257 1, builder %697, i257 1) #6
  %716 = tail call builder @llvm.tvm.stslice(slice %645, builder %715) #6
  br label %718

; <label>:717:                                    ; preds = %19
  tail call void @llvm.tvm.throw(i257 41) #6
  unreachable

; <label>:718:                                    ; preds = %714, %712
  %719 = phi builder [ %716, %714 ], [ %698, %712 ]
  %720 = tail call cell @llvm.tvm.endc(builder %719) #6
  %721 = tail call builder @llvm.tvm.stref(cell %720, builder %713) #6
  %722 = tail call cell @llvm.tvm.endc(builder %721) #6
  %723 = tail call builder @llvm.tvm.stref(cell %722, builder %708) #6
  %724 = tail call cell @llvm.tvm.endc(builder %723) #6
  tail call void @llvm.tvm.set.persistent.data(cell %724) #6
  br label %_ZN3tvm12smart_switchILb0E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit

_ZN3tvm12smart_switchILb0E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit: ; preds = %35, %163, %363, %459, %584, %718
  %725 = phi i257 [ 1756716863, %35 ], [ 1757739143, %163 ], [ 94113135, %363 ], [ 805461396, %459 ], [ 893474671, %584 ], [ 2112671963, %718 ]
  ret i257 %725
}

; Function Attrs: nounwind
define dso_local i257 @main_internal(cell, slice) local_unnamed_addr #0 personality i32 (...)* @__gxx_personality_v0 {
  %3 = tail call i257 @llvm.tvm.pushnull() #6
  %4 = tail call cell @llvm.tvm.cast.to.cell(i257 %3) #6
  %5 = tail call builder @llvm.tvm.newc() #6
  %6 = tail call builder @llvm.tvm.stu(i257 2, builder %5, i257 2) #6
  %7 = tail call builder @llvm.tvm.stu(i257 0, builder %6, i257 1) #6
  %8 = tail call builder @llvm.tvm.sti(i257 0, builder %7, i257 8) #6
  %9 = tail call builder @llvm.tvm.stu(i257 0, builder %8, i257 256) #6
  %10 = tail call cell @llvm.tvm.endc(builder %9) #6
  %11 = tail call slice @llvm.tvm.ctos(cell %10) #6
  %12 = tail call slice @llvm.tvm.ctos(cell %0) #6
  %13 = tail call { i257, slice } @llvm.tvm.ldu(slice %12, i257 1) #6
  %14 = extractvalue { i257, slice } %13, 0
  %15 = icmp eq i257 %14, 0
  br i1 %15, label %17, label %16

; <label>:16:                                     ; preds = %2
  tail call void @llvm.tvm.throw(i257 48) #6
  unreachable

; <label>:17:                                     ; preds = %2
  %18 = extractvalue { i257, slice } %13, 1
  %19 = tail call { slice, slice } @llvm.tvm.ldslice(slice %18, i257 2) #6
  %20 = extractvalue { slice, slice } %19, 1
  %21 = tail call { i257, slice } @llvm.tvm.ldu(slice %20, i257 1) #6
  %22 = extractvalue { i257, slice } %21, 0
  %23 = icmp eq i257 %22, 0
  br i1 %23, label %25, label %24

; <label>:24:                                     ; preds = %17
  tail call void @llvm.tvm.throw(i257 55) #6
  unreachable

; <label>:25:                                     ; preds = %17
  %26 = tail call { i257, slice } @llvm.tvm.ldu(slice %1, i257 32) #6
  %27 = extractvalue { i257, slice } %26, 0
  %28 = and i257 %27, 2147483648
  %29 = icmp eq i257 %28, 0
  %30 = extractvalue { i257, slice } %26, 1
  br i1 %29, label %329, label %31

; <label>:31:                                     ; preds = %25
  %32 = xor i257 %27, 2147483648
  %33 = tail call cell @llvm.tvm.get.persistent.data() #6
  %34 = tail call slice @llvm.tvm.ctos(cell %33) #6
  %35 = tail call { i257, slice } @llvm.tvm.ldu(slice %34, i257 1) #6
  %36 = extractvalue { i257, slice } %35, 0
  %37 = icmp eq i257 %36, 0
  br i1 %37, label %39, label %38

; <label>:38:                                     ; preds = %31
  tail call void @llvm.tvm.throw(i257 63) #6
  unreachable

; <label>:39:                                     ; preds = %31
  %40 = extractvalue { i257, slice } %35, 1
  %41 = tail call { i257, slice } @llvm.tvm.ldu(slice %40, i257 64) #6
  %42 = extractvalue { i257, slice } %41, 1
  %43 = extractvalue { i257, slice } %41, 0
  %44 = tail call { i257, slice } @llvm.tvm.ldu(slice %42, i257 32) #6
  %45 = extractvalue { i257, slice } %44, 1
  %46 = tail call { cell, slice } @llvm.tvm.lddict(slice %45) #6
  %47 = extractvalue { i257, slice } %44, 0
  %48 = extractvalue { cell, slice } %46, 0
  %49 = extractvalue { cell, slice } %46, 1
  %50 = tail call { i257, slice } @llvm.tvm.ldu(slice %49, i257 1) #6
  %51 = extractvalue { i257, slice } %50, 0
  %52 = extractvalue { i257, slice } %50, 1
  %53 = icmp eq i257 %51, 0
  br i1 %53, label %58, label %54

; <label>:54:                                     ; preds = %39
  %55 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %52) #6
  %56 = extractvalue { slice, slice } %55, 0
  %57 = extractvalue { slice, slice } %55, 1
  br label %58

; <label>:58:                                     ; preds = %54, %39
  %59 = phi slice [ bitcast (i257 0 to slice), %39 ], [ %56, %54 ]
  %60 = phi i257 [ 0, %39 ], [ 1, %54 ]
  %61 = phi slice [ %52, %39 ], [ %57, %54 ]
  %62 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %61) #6
  %63 = extractvalue { slice, slice } %62, 1
  %64 = tail call { i257, slice } @llvm.tvm.ldu(slice %63, i257 1) #6
  %65 = extractvalue { i257, slice } %64, 0
  %66 = extractvalue { i257, slice } %64, 1
  %67 = icmp eq i257 %65, 0
  br i1 %67, label %72, label %68

; <label>:68:                                     ; preds = %58
  %69 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %66) #6
  %70 = extractvalue { slice, slice } %69, 0
  %71 = extractvalue { slice, slice } %69, 1
  br label %72

; <label>:72:                                     ; preds = %68, %58
  %73 = phi slice [ bitcast (i257 0 to slice), %58 ], [ %70, %68 ]
  %74 = phi i257 [ 0, %58 ], [ 1, %68 ]
  %75 = phi slice [ %66, %58 ], [ %71, %68 ]
  %76 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %75) #6
  %77 = extractvalue { slice, slice } %76, 1
  %78 = tail call { i257, slice } @llvm.tvm.ldu(slice %77, i257 1) #6
  %79 = extractvalue { i257, slice } %78, 0
  %80 = icmp eq i257 %79, 0
  br i1 %80, label %85, label %81

; <label>:81:                                     ; preds = %72
  %82 = extractvalue { i257, slice } %78, 1
  %83 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %82) #6
  %84 = extractvalue { slice, slice } %83, 0
  br label %85

; <label>:85:                                     ; preds = %81, %72
  %86 = phi slice [ bitcast (i257 0 to slice), %72 ], [ %84, %81 ]
  %87 = phi i257 [ 0, %72 ], [ 1, %81 ]
  %88 = tail call { cell, i257 } @llvm.tvm.dictugetref(i257 %32, cell %48, i257 32) #6
  %89 = extractvalue { cell, i257 } %88, 1
  %90 = icmp eq i257 %89, 0
  br i1 %90, label %91, label %92

; <label>:91:                                     ; preds = %85
  tail call void @llvm.tvm.throw(i257 59) #6
  unreachable

; <label>:92:                                     ; preds = %85
  %93 = extractvalue { cell, i257 } %88, 0
  %94 = tail call slice @llvm.tvm.ctos(cell %93) #6
  %95 = tail call { i257, slice } @llvm.tvm.ldu(slice %94, i257 32) #6
  %96 = extractvalue { i257, slice } %95, 1
  %97 = tail call { i257, slice } @llvm.tvm.ldi(slice %96, i257 8) #6
  %98 = extractvalue { i257, slice } %97, 1
  %99 = tail call { i257, slice } @llvm.tvm.ldu(slice %98, i257 256) #6
  %100 = extractvalue { i257, slice } %99, 1
  %101 = tail call { slice, slice } @llvm.tvm.ldrefrtos(slice %100) #6
  %102 = extractvalue { slice, slice } %101, 1
  %103 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %102) #6
  %104 = extractvalue { slice, slice } %103, 1
  tail call void @llvm.tvm.ends(slice %104) #6
  %105 = extractvalue { i257, slice } %95, 0
  %106 = extractvalue { i257, slice } %97, 0
  %107 = extractvalue { i257, slice } %99, 0
  %108 = extractvalue { slice, slice } %103, 0
  %109 = tail call { i257, slice } @llvm.tvm.ldu(slice %18, i257 1) #6
  %110 = extractvalue { i257, slice } %109, 1
  %111 = tail call { i257, slice } @llvm.tvm.ldu(slice %110, i257 1) #6
  %112 = extractvalue { i257, slice } %111, 1
  %113 = tail call { i257, slice } @llvm.tvm.ldu(slice %112, i257 1) #6
  %114 = extractvalue { i257, slice } %113, 1
  %115 = tail call { slice, slice } @llvm.tvm.ldmsgaddr(slice %114) #6
  %116 = extractvalue { slice, slice } %115, 0
  %117 = tail call { i257, slice } @llvm.tvm.ldu(slice %116, i257 2) #6
  %118 = extractvalue { i257, slice } %117, 0
  switch i257 %118, label %129 [
    i257 2, label %119
    i257 3, label %128
  ]

; <label>:119:                                    ; preds = %92
  %120 = extractvalue { i257, slice } %117, 1
  %121 = tail call { i257, slice } @llvm.tvm.ldu(slice %120, i257 1) #6
  %122 = extractvalue { i257, slice } %121, 0
  %123 = extractvalue { i257, slice } %121, 1
  %124 = icmp eq i257 %122, 0
  br i1 %124, label %130, label %125

; <label>:125:                                    ; preds = %119
  %126 = tail call { i257, slice } @llvm.tvm.ldvaruint32(slice %123) #6
  %127 = extractvalue { i257, slice } %126, 1
  br label %130

; <label>:128:                                    ; preds = %92
  tail call void @llvm.tvm.throw(i257 99) #6
  unreachable

; <label>:129:                                    ; preds = %92
  tail call void @llvm.tvm.throw(i257 52) #6
  unreachable

; <label>:130:                                    ; preds = %125, %119
  %131 = phi slice [ %123, %119 ], [ %127, %125 ]
  %132 = tail call { i257, slice } @llvm.tvm.ldi(slice %131, i257 8) #6
  %133 = extractvalue { i257, slice } %132, 1
  %134 = extractvalue { i257, slice } %132, 0
  %135 = tail call { i257, slice } @llvm.tvm.ldu(slice %133, i257 256) #6
  %136 = extractvalue { i257, slice } %135, 0
  %137 = icmp eq i257 %134, %106
  %138 = icmp eq i257 %136, %107
  %139 = and i1 %137, %138
  br i1 %139, label %141, label %140

; <label>:140:                                    ; preds = %130
  tail call void @llvm.tvm.throw(i257 59) #6
  unreachable

; <label>:141:                                    ; preds = %130
  %142 = tail call { cell, i257 } @llvm.tvm.dictudel(i257 %32, cell %48, i257 32) #6
  %143 = extractvalue { cell, i257 } %142, 0
  %144 = icmp eq i257 %105, 94113135
  br i1 %144, label %145, label %328

; <label>:145:                                    ; preds = %141
  %146 = tail call i257 @llvm.tvm.cast.from.slice(slice %30) #6
  tail call void @llvm.tvm.setglobal(i257 3, i257 %146) #6
  tail call void @llvm.tvm.accept() #6
  %147 = tail call i257 @llvm.tvm.getglobal(i257 3) #6
  %148 = icmp eq i257 %60, 0
  br i1 %148, label %149, label %177

; <label>:149:                                    ; preds = %145
  %150 = tail call builder @llvm.tvm.newc() #6
  %151 = tail call builder @llvm.tvm.stu(i257 36376547073305762578600252039007217027168892248167876973455473303899333671434, builder %150, i257 256) #6
  %152 = tail call builder @llvm.tvm.stu(i257 0, builder %151, i257 8) #6
  %153 = tail call cell @llvm.tvm.endc(builder %152) #6
  %154 = tail call builder @llvm.tvm.stu(i257 0, builder %150, i257 2) #6
  %155 = tail call cell @llvm.tvm.endc(builder %154) #6
  %156 = tail call slice @llvm.tvm.ctos(cell %155) #6
  %157 = tail call builder @llvm.tvm.stu(i257 0, builder %150, i257 1) #6
  %158 = tail call builder @llvm.tvm.stu(i257 1, builder %157, i257 1) #6
  %159 = tail call builder @llvm.tvm.stu(i257 1, builder %158, i257 1) #6
  %160 = tail call builder @llvm.tvm.stu(i257 0, builder %159, i257 1) #6
  %161 = tail call builder @llvm.tvm.stslice(slice %156, builder %160) #6
  %162 = tail call builder @llvm.tvm.stslice(slice %11, builder %161) #6
  %163 = tail call builder @llvm.tvm.stvaruint16(builder %162, i257 10000000) #6
  %164 = tail call builder @llvm.tvm.stdict(cell %4, builder %163) #6
  %165 = tail call builder @llvm.tvm.stvaruint16(builder %164, i257 0) #6
  %166 = tail call builder @llvm.tvm.stvaruint16(builder %165, i257 0) #6
  %167 = tail call builder @llvm.tvm.stu(i257 0, builder %166, i257 64) #6
  %168 = tail call builder @llvm.tvm.stu(i257 0, builder %167, i257 32) #6
  %169 = tail call builder @llvm.tvm.stu(i257 0, builder %168, i257 1) #6
  %170 = tail call builder @llvm.tvm.stu(i257 1, builder %169, i257 1) #6
  %171 = tail call builder @llvm.tvm.stu(i257 273758626, builder %150, i257 32) #6
  %172 = tail call builder @llvm.tvm.stref(cell %153, builder %171) #6
  %173 = tail call cell @llvm.tvm.endc(builder %172) #6
  %174 = tail call builder @llvm.tvm.stref(cell %173, builder %170) #6
  %175 = tail call cell @llvm.tvm.endc(builder %174) #6
  tail call void @llvm.tvm.sendrawmsg(cell %175, i257 3) #6
  %176 = tail call i257 @llvm.tvm.cast.from.slice(slice %11) #6
  tail call void @llvm.tvm.setglobal(i257 2, i257 %176) #6
  br label %238

; <label>:177:                                    ; preds = %145
  %178 = icmp eq i257 %74, 0
  %179 = tail call builder @llvm.tvm.newc() #6
  br i1 %178, label %180, label %206

; <label>:180:                                    ; preds = %177
  %181 = tail call builder @llvm.tvm.stu(i257 37740553060181980809513498422423267373287682858697291256796625716503165667840, builder %179, i257 256) #6
  %182 = tail call cell @llvm.tvm.endc(builder %181) #6
  %183 = tail call builder @llvm.tvm.stu(i257 0, builder %179, i257 2) #6
  %184 = tail call cell @llvm.tvm.endc(builder %183) #6
  %185 = tail call slice @llvm.tvm.ctos(cell %184) #6
  %186 = tail call builder @llvm.tvm.stu(i257 0, builder %179, i257 1) #6
  %187 = tail call builder @llvm.tvm.stu(i257 1, builder %186, i257 1) #6
  %188 = tail call builder @llvm.tvm.stu(i257 1, builder %187, i257 1) #6
  %189 = tail call builder @llvm.tvm.stu(i257 0, builder %188, i257 1) #6
  %190 = tail call builder @llvm.tvm.stslice(slice %185, builder %189) #6
  %191 = tail call builder @llvm.tvm.stslice(slice %11, builder %190) #6
  %192 = tail call builder @llvm.tvm.stvaruint16(builder %191, i257 10000000) #6
  %193 = tail call builder @llvm.tvm.stdict(cell %4, builder %192) #6
  %194 = tail call builder @llvm.tvm.stvaruint16(builder %193, i257 0) #6
  %195 = tail call builder @llvm.tvm.stvaruint16(builder %194, i257 0) #6
  %196 = tail call builder @llvm.tvm.stu(i257 0, builder %195, i257 64) #6
  %197 = tail call builder @llvm.tvm.stu(i257 0, builder %196, i257 32) #6
  %198 = tail call builder @llvm.tvm.stu(i257 0, builder %197, i257 1) #6
  %199 = tail call builder @llvm.tvm.stu(i257 1, builder %198, i257 1) #6
  %200 = tail call builder @llvm.tvm.stu(i257 273758626, builder %179, i257 32) #6
  %201 = tail call builder @llvm.tvm.stref(cell %182, builder %200) #6
  %202 = tail call cell @llvm.tvm.endc(builder %201) #6
  %203 = tail call builder @llvm.tvm.stref(cell %202, builder %199) #6
  %204 = tail call cell @llvm.tvm.endc(builder %203) #6
  tail call void @llvm.tvm.sendrawmsg(cell %204, i257 3) #6
  %205 = tail call i257 @llvm.tvm.cast.from.slice(slice %11) #6
  tail call void @llvm.tvm.setglobal(i257 2, i257 %205) #6
  br label %238

; <label>:206:                                    ; preds = %177
  %207 = tail call builder @llvm.tvm.stu(i257 2057670211779114192786016431245824, builder %179, i257 112) #6
  %208 = tail call cell @llvm.tvm.endc(builder %207) #6
  %209 = tail call builder @llvm.tvm.stref(cell %208, builder %179) #6
  %210 = tail call builder @llvm.tvm.stu(i257 22236088660389195836712816582366228804315807652401902655158701366908164400481, builder %209, i257 256) #6
  %211 = tail call builder @llvm.tvm.stu(i257 52646965497419283711209620731437176695766625418528417975960905999894446564640, builder %210, i257 256) #6
  %212 = tail call builder @llvm.tvm.stu(i257 51760553987291134220425779321469767883269085913013081901329045939596320532839, builder %211, i257 256) #6
  %213 = tail call builder @llvm.tvm.stu(i257 2565442613337978162052422250016, builder %212, i257 104) #6
  %214 = tail call cell @llvm.tvm.endc(builder %213) #6
  %215 = tail call builder @llvm.tvm.stu(i257 0, builder %179, i257 2) #6
  %216 = tail call cell @llvm.tvm.endc(builder %215) #6
  %217 = tail call slice @llvm.tvm.ctos(cell %216) #6
  %218 = tail call builder @llvm.tvm.stu(i257 0, builder %179, i257 1) #6
  %219 = tail call builder @llvm.tvm.stu(i257 1, builder %218, i257 1) #6
  %220 = tail call builder @llvm.tvm.stu(i257 1, builder %219, i257 1) #6
  %221 = tail call builder @llvm.tvm.stu(i257 0, builder %220, i257 1) #6
  %222 = tail call builder @llvm.tvm.stslice(slice %217, builder %221) #6
  %223 = tail call builder @llvm.tvm.stslice(slice %11, builder %222) #6
  %224 = tail call builder @llvm.tvm.stvaruint16(builder %223, i257 10000000) #6
  %225 = tail call builder @llvm.tvm.stdict(cell %4, builder %224) #6
  %226 = tail call builder @llvm.tvm.stvaruint16(builder %225, i257 0) #6
  %227 = tail call builder @llvm.tvm.stvaruint16(builder %226, i257 0) #6
  %228 = tail call builder @llvm.tvm.stu(i257 0, builder %227, i257 64) #6
  %229 = tail call builder @llvm.tvm.stu(i257 0, builder %228, i257 32) #6
  %230 = tail call builder @llvm.tvm.stu(i257 0, builder %229, i257 1) #6
  %231 = tail call builder @llvm.tvm.stu(i257 1, builder %230, i257 1) #6
  %232 = tail call builder @llvm.tvm.stu(i257 2122322724, builder %179, i257 32) #6
  %233 = tail call builder @llvm.tvm.stref(cell %214, builder %232) #6
  %234 = tail call cell @llvm.tvm.endc(builder %233) #6
  %235 = tail call builder @llvm.tvm.stref(cell %234, builder %231) #6
  %236 = tail call cell @llvm.tvm.endc(builder %235) #6
  tail call void @llvm.tvm.sendrawmsg(cell %236, i257 3) #6
  %237 = tail call i257 @llvm.tvm.cast.from.slice(slice %11) #6
  tail call void @llvm.tvm.setglobal(i257 2, i257 %237) #6
  br label %238

; <label>:238:                                    ; preds = %206, %180, %149
  %239 = phi i257 [ 3, %206 ], [ 2, %180 ], [ 1, %149 ]
  %240 = tail call i257 @llvm.tvm.getglobal(i257 2) #6
  %241 = tail call slice @llvm.tvm.cast.to.slice(i257 %240) #6
  %242 = tail call { i257, slice } @llvm.tvm.ldu(slice %241, i257 2) #6
  %243 = extractvalue { i257, slice } %242, 0
  switch i257 %243, label %254 [
    i257 2, label %244
    i257 3, label %253
  ]

; <label>:244:                                    ; preds = %238
  %245 = extractvalue { i257, slice } %242, 1
  %246 = tail call { i257, slice } @llvm.tvm.ldu(slice %245, i257 1) #6
  %247 = extractvalue { i257, slice } %246, 0
  %248 = extractvalue { i257, slice } %246, 1
  %249 = icmp eq i257 %247, 0
  br i1 %249, label %255, label %250

; <label>:250:                                    ; preds = %244
  %251 = tail call { i257, slice } @llvm.tvm.ldvaruint32(slice %248) #6
  %252 = extractvalue { i257, slice } %251, 1
  br label %255

; <label>:253:                                    ; preds = %238
  tail call void @llvm.tvm.throw(i257 99) #6
  unreachable

; <label>:254:                                    ; preds = %238
  tail call void @llvm.tvm.throw(i257 52) #6
  unreachable

; <label>:255:                                    ; preds = %250, %244
  %256 = phi slice [ %248, %244 ], [ %252, %250 ]
  %257 = tail call { i257, slice } @llvm.tvm.ldi(slice %256, i257 8) #6
  %258 = tail call builder @llvm.tvm.newc() #6
  %259 = extractvalue { i257, slice } %257, 1
  %260 = tail call { i257, slice } @llvm.tvm.ldu(slice %259, i257 256) #6
  %261 = extractvalue { i257, slice } %260, 0
  %262 = extractvalue { i257, slice } %257, 0
  %263 = tail call cell @llvm.tvm.endc(builder undef) #6
  %264 = tail call builder @llvm.tvm.sti(i257 undef, builder %258, i257 257) #6
  %265 = tail call builder @llvm.tvm.sti(i257 undef, builder %264, i257 257) #6
  %266 = tail call builder @llvm.tvm.stref(cell undef, builder %265) #6
  %267 = tail call builder @llvm.tvm.stref(cell %263, builder %266) #6
  %268 = tail call cell @llvm.tvm.endc(builder %267) #6
  %269 = tail call builder @llvm.tvm.stslice(slice undef, builder %258) #6
  %270 = tail call cell @llvm.tvm.endc(builder %269) #6
  %271 = tail call builder @llvm.tvm.sti(i257 0, builder %258, i257 257) #6
  %272 = tail call builder @llvm.tvm.stref(cell undef, builder %271) #6
  %273 = tail call builder @llvm.tvm.sti(i257 undef, builder %272, i257 257) #6
  %274 = tail call builder @llvm.tvm.stref(cell %263, builder %273) #6
  %275 = tail call builder @llvm.tvm.stref(cell %270, builder %274) #6
  %276 = tail call builder @llvm.tvm.sti(i257 undef, builder %275, i257 257) #6
  %277 = tail call builder @llvm.tvm.stref(cell %268, builder %276) #6
  %278 = tail call cell @llvm.tvm.endc(builder %277) #6
  %279 = tail call builder @llvm.tvm.stref(cell undef, builder %264) #6
  %280 = tail call builder @llvm.tvm.sti(i257 0, builder %279, i257 257) #6
  %281 = tail call builder @llvm.tvm.sti(i257 0, builder %280, i257 257) #6
  %282 = tail call builder @llvm.tvm.stref(cell undef, builder %281) #6
  %283 = tail call builder @llvm.tvm.stref(cell undef, builder %282) #6
  %284 = tail call builder @llvm.tvm.stref(cell %278, builder %283) #6
  %285 = tail call cell @llvm.tvm.endc(builder %284) #6
  %286 = tail call builder @llvm.tvm.sti(i257 0, builder %264, i257 257) #6
  %287 = tail call builder @llvm.tvm.sti(i257 undef, builder %286, i257 257) #6
  %288 = tail call builder @llvm.tvm.stref(cell %285, builder %287) #6
  %289 = tail call cell @llvm.tvm.endc(builder %288) #6
  %290 = tail call builder @llvm.tvm.sti(i257 undef, builder %266, i257 257) #6
  %291 = tail call builder @llvm.tvm.stref(cell %289, builder %290) #6
  %292 = tail call cell @llvm.tvm.endc(builder %291) #6
  %293 = tail call builder @llvm.tvm.stslice(slice %108, builder %258) #6
  %294 = tail call cell @llvm.tvm.endc(builder %293) #6
  %295 = tail call builder @llvm.tvm.stu(i257 94113135, builder %258, i257 32) #6
  %296 = tail call builder @llvm.tvm.sti(i257 %262, builder %295, i257 8) #6
  %297 = tail call builder @llvm.tvm.stu(i257 %261, builder %296, i257 256) #6
  %298 = tail call builder @llvm.tvm.stref(cell %294, builder %297) #6
  %299 = tail call builder @llvm.tvm.stu(i257 %239, builder %298, i257 6) #6
  %300 = tail call builder @llvm.tvm.sti(i257 undef, builder %299, i257 257) #6
  %301 = tail call builder @llvm.tvm.stref(cell %270, builder %300) #6
  %302 = tail call builder @llvm.tvm.sti(i257 undef, builder %301, i257 257) #6
  %303 = tail call builder @llvm.tvm.stref(cell %292, builder %302) #6
  %304 = tail call cell @llvm.tvm.endc(builder %303) #6
  %305 = tail call cell @llvm.tvm.dictusetref(cell %304, i257 94113135, cell %143, i257 32) #6
  %306 = icmp eq i257 %74, 0
  %307 = icmp eq i257 %87, 0
  %308 = tail call builder @llvm.tvm.newc() #6
  %309 = tail call builder @llvm.tvm.stu(i257 0, builder %308, i257 1) #6
  %310 = tail call builder @llvm.tvm.stu(i257 %43, builder %309, i257 64) #6
  %311 = tail call builder @llvm.tvm.stu(i257 %47, builder %310, i257 32) #6
  %312 = tail call builder @llvm.tvm.stdict(cell %305, builder %311) #6
  br i1 %148, label %316, label %313

; <label>:313:                                    ; preds = %255
  %314 = tail call builder @llvm.tvm.stu(i257 1, builder %312, i257 1) #6
  %315 = tail call builder @llvm.tvm.stslice(slice %59, builder %314) #6
  br label %318

; <label>:316:                                    ; preds = %255
  %317 = tail call builder @llvm.tvm.stu(i257 0, builder %312, i257 1) #6
  br label %318

; <label>:318:                                    ; preds = %316, %313
  %319 = phi builder [ %315, %313 ], [ %317, %316 ]
  br i1 %306, label %323, label %320

; <label>:320:                                    ; preds = %318
  %321 = tail call builder @llvm.tvm.stu(i257 1, builder %308, i257 1) #6
  %322 = tail call builder @llvm.tvm.stslice(slice %73, builder %321) #6
  br label %323

; <label>:323:                                    ; preds = %320, %318
  %324 = phi builder [ %322, %320 ], [ %309, %318 ]
  br i1 %307, label %_ZN3tvm12smart_switchILb1E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit, label %325

; <label>:325:                                    ; preds = %323
  %326 = tail call builder @llvm.tvm.stu(i257 1, builder %308, i257 1) #6
  %327 = tail call builder @llvm.tvm.stslice(slice %86, builder %326) #6
  br label %_ZN3tvm12smart_switchILb1E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit

; <label>:328:                                    ; preds = %141
  tail call void @llvm.tvm.throw(i257 58) #6
  unreachable

; <label>:329:                                    ; preds = %25
  tail call void @llvm.tvm.throw(i257 41) #6
  unreachable

_ZN3tvm12smart_switchILb1E9SMV_Debot10ISMV_Debot10DSMV_DebotNS_24replay_attack_protection9timestampILj1800EEEEEiNS_4cellENS_5sliceE.exit: ; preds = %323, %325
  %330 = phi builder [ %327, %325 ], [ %309, %323 ]
  %331 = tail call cell @llvm.tvm.endc(builder %330) #6
  %332 = tail call builder @llvm.tvm.stref(cell %331, builder %324) #6
  %333 = tail call cell @llvm.tvm.endc(builder %332) #6
  %334 = tail call builder @llvm.tvm.stref(cell %333, builder %319) #6
  %335 = tail call cell @llvm.tvm.endc(builder %334) #6
  tail call void @llvm.tvm.set.persistent.data(cell %335) #6
  ret i257 94113135
}

; Function Attrs: noreturn nounwind
define dso_local i257 @main_ticktock(cell, slice) local_unnamed_addr #1 {
  tail call void @llvm.tvm.throw(i257 54) #6
  unreachable
}

; Function Attrs: noreturn nounwind
define dso_local i257 @main_split(cell, slice) local_unnamed_addr #1 {
  tail call void @llvm.tvm.throw(i257 54) #6
  unreachable
}

; Function Attrs: noreturn nounwind
define dso_local i257 @main_merge(cell, slice) local_unnamed_addr #1 {
  tail call void @llvm.tvm.throw(i257 54) #6
  unreachable
}

; Function Attrs: noreturn nounwind
declare void @llvm.tvm.throw(i257) #2

; Function Attrs: nounwind readnone
declare cell @llvm.tvm.cast.to.cell(i257) #3

; Function Attrs: nounwind readnone
declare i257 @llvm.tvm.pushnull() #3

declare dso_local i32 @__gxx_personality_v0(...)

; Function Attrs: nounwind readnone
declare slice @llvm.tvm.cast.to.slice(i257) #3

; Function Attrs: nounwind readnone
declare builder @llvm.tvm.stu(i257, builder, i257) #3

; Function Attrs: nounwind readnone
declare builder @llvm.tvm.sti(i257, builder, i257) #3

; Function Attrs: nounwind readnone
declare builder @llvm.tvm.stslice(slice, builder) #3

; Function Attrs: inaccessiblememonly nounwind readonly
declare builder @llvm.tvm.newc() #4

; Function Attrs: nounwind readnone
declare cell @llvm.tvm.endc(builder) #3

; Function Attrs: nounwind readnone
declare slice @llvm.tvm.ctos(cell) #3

; Function Attrs: nounwind readnone
declare i257 @llvm.tvm.cast.from.slice(slice) #3

; Function Attrs: inaccessiblememonly nounwind
declare void @llvm.tvm.sendrawmsg(cell, i257) #5

; Function Attrs: nounwind readnone
declare builder @llvm.tvm.stvaruint16(builder, i257) #3

; Function Attrs: nounwind readnone
declare builder @llvm.tvm.stdict(cell, builder) #3

; Function Attrs: nounwind readnone
declare builder @llvm.tvm.stref(cell, builder) #3

; Function Attrs: nounwind
declare void @llvm.tvm.setglobal(i257, i257) #6

; Function Attrs: nounwind readnone
declare { i257, slice } @llvm.tvm.ldu(slice, i257) #3

; Function Attrs: nounwind
declare void @llvm.tvm.ends(slice) #6

; Function Attrs: nounwind
declare i257 @llvm.tvm.getglobal(i257) #6

; Function Attrs: nounwind readnone
declare { slice, slice } @llvm.tvm.ldmsgaddr(slice) #3

; Function Attrs: nounwind readnone
declare { slice, slice } @llvm.tvm.ldslice(slice, i257) #3

; Function Attrs: nounwind
declare cell @llvm.tvm.get.persistent.data() #6

; Function Attrs: nounwind
declare void @llvm.tvm.accept() #6

; Function Attrs: nounwind
declare void @llvm.tvm.set.persistent.data(cell) #6

; Function Attrs: nounwind readnone
declare { cell, slice } @llvm.tvm.lddict(slice) #3

; Function Attrs: nounwind readnone
declare { slice, slice } @llvm.tvm.ldrefrtos(slice) #3

; Function Attrs: nounwind readnone
declare i257 @llvm.tvm.now() #3

; Function Attrs: nounwind
declare { cell, i257 } @llvm.tvm.dictugetref(i257, cell, i257) #6

; Function Attrs: nounwind readnone
declare { i257, slice } @llvm.tvm.ldvaruint32(slice) #3

; Function Attrs: nounwind readnone
declare { i257, slice } @llvm.tvm.ldi(slice, i257) #3

; Function Attrs: nounwind
declare cell @llvm.tvm.dictusetref(cell, i257, cell, i257) #6

; Function Attrs: nounwind
declare { cell, i257 } @llvm.tvm.dictudel(i257, cell, i257) #6

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "tvm_raw_func" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noreturn nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "tvm_raw_func" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noreturn nounwind }
attributes #3 = { nounwind readnone }
attributes #4 = { inaccessiblememonly nounwind readonly }
attributes #5 = { inaccessiblememonly nounwind }
attributes #6 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{!"TON Labs clang for TVM. Version 7.0.0  (based on LLVM 7.0.0)"}
!2 = !{!3}
!3 = distinct !{!3, !4, !"_ZN9SMV_Debot5startEv: argument 0"}
!4 = distinct !{!4, !"_ZN9SMV_Debot5startEv"}
