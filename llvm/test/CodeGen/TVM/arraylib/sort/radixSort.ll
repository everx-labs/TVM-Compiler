; XFAIL: *
; RUN: llc < %s -march=tvm
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define dso_local i64 @largest(i64* nocapture readonly, i64) local_unnamed_addr #0 {
  %3 = load i64, i64* %0, align 8, !tbaa !4
  %4 = icmp sgt i64 %1, 1
  br i1 %4, label %5, label %54

; <label>:5:                                      ; preds = %2
  %6 = add i64 %1, -1
  %7 = add i64 %1, -2
  %8 = and i64 %6, 3
  %9 = icmp ult i64 %7, 3
  br i1 %9, label %38, label %10

; <label>:10:                                     ; preds = %5
  %11 = sub i64 %6, %8
  br label %12

; <label>:12:                                     ; preds = %12, %10
  %13 = phi i64 [ 1, %10 ], [ %35, %12 ]
  %14 = phi i64 [ %3, %10 ], [ %34, %12 ]
  %15 = phi i64 [ %11, %10 ], [ %36, %12 ]
  %16 = getelementptr inbounds i64, i64* %0, i64 %13
  %17 = load i64, i64* %16, align 8, !tbaa !4
  %18 = icmp slt i64 %14, %17
  %19 = select i1 %18, i64 %17, i64 %14
  %20 = add nuw nsw i64 %13, 1
  %21 = getelementptr inbounds i64, i64* %0, i64 %20
  %22 = load i64, i64* %21, align 8, !tbaa !4
  %23 = icmp slt i64 %19, %22
  %24 = select i1 %23, i64 %22, i64 %19
  %25 = add nuw nsw i64 %13, 2
  %26 = getelementptr inbounds i64, i64* %0, i64 %25
  %27 = load i64, i64* %26, align 8, !tbaa !4
  %28 = icmp slt i64 %24, %27
  %29 = select i1 %28, i64 %27, i64 %24
  %30 = add nuw nsw i64 %13, 3
  %31 = getelementptr inbounds i64, i64* %0, i64 %30
  %32 = load i64, i64* %31, align 8, !tbaa !4
  %33 = icmp slt i64 %29, %32
  %34 = select i1 %33, i64 %32, i64 %29
  %35 = add nuw nsw i64 %13, 4
  %36 = add i64 %15, -4
  %37 = icmp eq i64 %36, 0
  br i1 %37, label %38, label %12

; <label>:38:                                     ; preds = %12, %5
  %39 = phi i64 [ undef, %5 ], [ %34, %12 ]
  %40 = phi i64 [ 1, %5 ], [ %35, %12 ]
  %41 = phi i64 [ %3, %5 ], [ %34, %12 ]
  %42 = icmp eq i64 %8, 0
  br i1 %42, label %54, label %43

; <label>:43:                                     ; preds = %38, %43
  %44 = phi i64 [ %51, %43 ], [ %40, %38 ]
  %45 = phi i64 [ %50, %43 ], [ %41, %38 ]
  %46 = phi i64 [ %52, %43 ], [ %8, %38 ]
  %47 = getelementptr inbounds i64, i64* %0, i64 %44
  %48 = load i64, i64* %47, align 8, !tbaa !4
  %49 = icmp slt i64 %45, %48
  %50 = select i1 %49, i64 %48, i64 %45
  %51 = add nuw nsw i64 %44, 1
  %52 = add i64 %46, -1
  %53 = icmp eq i64 %52, 0
  br i1 %53, label %54, label %43, !llvm.loop !8

; <label>:54:                                     ; preds = %38, %43, %2
  %55 = phi i64 [ %3, %2 ], [ %39, %38 ], [ %50, %43 ]
  ret i64 %55
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #1

; Function Attrs: nounwind sspstrong uwtable
define dso_local void @RadixSort(i64* nocapture, i64) local_unnamed_addr #2 {
  %3 = alloca [10 x [10 x i64]], align 16
  %4 = alloca [10 x i64], align 16
  %5 = bitcast [10 x i64]* %4 to i8*
  %6 = bitcast [10 x [10 x i64]]* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 800, i8* nonnull %6) #3
  call void @llvm.lifetime.start.p0i8(i64 80, i8* nonnull %5) #3
  %7 = load i64, i64* %0, align 8, !tbaa !4
  %8 = icmp sgt i64 %1, 1
  br i1 %8, label %9, label %58

; <label>:9:                                      ; preds = %2
  %10 = add i64 %1, -1
  %11 = add i64 %1, -2
  %12 = and i64 %10, 3
  %13 = icmp ult i64 %11, 3
  br i1 %13, label %42, label %14

; <label>:14:                                     ; preds = %9
  %15 = sub i64 %10, %12
  br label %16

; <label>:16:                                     ; preds = %16, %14
  %17 = phi i64 [ 1, %14 ], [ %39, %16 ]
  %18 = phi i64 [ %7, %14 ], [ %38, %16 ]
  %19 = phi i64 [ %15, %14 ], [ %40, %16 ]
  %20 = getelementptr inbounds i64, i64* %0, i64 %17
  %21 = load i64, i64* %20, align 8, !tbaa !4
  %22 = icmp slt i64 %18, %21
  %23 = select i1 %22, i64 %21, i64 %18
  %24 = add nuw nsw i64 %17, 1
  %25 = getelementptr inbounds i64, i64* %0, i64 %24
  %26 = load i64, i64* %25, align 8, !tbaa !4
  %27 = icmp slt i64 %23, %26
  %28 = select i1 %27, i64 %26, i64 %23
  %29 = add nuw nsw i64 %17, 2
  %30 = getelementptr inbounds i64, i64* %0, i64 %29
  %31 = load i64, i64* %30, align 8, !tbaa !4
  %32 = icmp slt i64 %28, %31
  %33 = select i1 %32, i64 %31, i64 %28
  %34 = add nuw nsw i64 %17, 3
  %35 = getelementptr inbounds i64, i64* %0, i64 %34
  %36 = load i64, i64* %35, align 8, !tbaa !4
  %37 = icmp slt i64 %33, %36
  %38 = select i1 %37, i64 %36, i64 %33
  %39 = add nuw nsw i64 %17, 4
  %40 = add i64 %19, -4
  %41 = icmp eq i64 %40, 0
  br i1 %41, label %42, label %16

; <label>:42:                                     ; preds = %16, %9
  %43 = phi i64 [ undef, %9 ], [ %38, %16 ]
  %44 = phi i64 [ 1, %9 ], [ %39, %16 ]
  %45 = phi i64 [ %7, %9 ], [ %38, %16 ]
  %46 = icmp eq i64 %12, 0
  br i1 %46, label %58, label %47

; <label>:47:                                     ; preds = %42, %47
  %48 = phi i64 [ %55, %47 ], [ %44, %42 ]
  %49 = phi i64 [ %54, %47 ], [ %45, %42 ]
  %50 = phi i64 [ %56, %47 ], [ %12, %42 ]
  %51 = getelementptr inbounds i64, i64* %0, i64 %48
  %52 = load i64, i64* %51, align 8, !tbaa !4
  %53 = icmp slt i64 %49, %52
  %54 = select i1 %53, i64 %52, i64 %49
  %55 = add nuw nsw i64 %48, 1
  %56 = add i64 %50, -1
  %57 = icmp eq i64 %56, 0
  br i1 %57, label %58, label %47, !llvm.loop !10

; <label>:58:                                     ; preds = %42, %47, %2
  %59 = phi i64 [ %7, %2 ], [ %43, %42 ], [ %54, %47 ]
  %60 = icmp sgt i64 %59, 0
  br i1 %60, label %144, label %156

; <label>:61:                                     ; preds = %144
  %62 = icmp sgt i64 %1, 0
  br i1 %62, label %68, label %63

; <label>:63:                                     ; preds = %61
  %64 = and i64 %147, 7
  %65 = icmp ult i64 %146, 7
  br i1 %65, label %150, label %66

; <label>:66:                                     ; preds = %63
  %67 = sub nsw i64 %147, %64
  br label %412

; <label>:68:                                     ; preds = %61
  %69 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 0
  %70 = bitcast [10 x [10 x i64]]* %3 to i8*
  %71 = bitcast i64* %0 to i8*
  %72 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 1
  %73 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 1, i64 0
  %74 = bitcast i64* %73 to i8*
  %75 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 2
  %76 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 2, i64 0
  %77 = bitcast i64* %76 to i8*
  %78 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 3
  %79 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 3, i64 0
  %80 = bitcast i64* %79 to i8*
  %81 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 4
  %82 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 4, i64 0
  %83 = bitcast i64* %82 to i8*
  %84 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 5
  %85 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 5, i64 0
  %86 = bitcast i64* %85 to i8*
  %87 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 6
  %88 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 6, i64 0
  %89 = bitcast i64* %88 to i8*
  %90 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 7
  %91 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 7, i64 0
  %92 = bitcast i64* %91 to i8*
  %93 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 8
  %94 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 8, i64 0
  %95 = bitcast i64* %94 to i8*
  %96 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 9
  %97 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 9, i64 0
  %98 = bitcast i64* %97 to i8*
  br label %99

; <label>:99:                                     ; preds = %408, %68
  %100 = phi i64 [ %410, %408 ], [ 0, %68 ]
  %101 = phi i64 [ %409, %408 ], [ 1, %68 ]
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  br label %122

; <label>:102:                                    ; preds = %116, %137
  %103 = phi i64 [ undef, %137 ], [ %119, %116 ]
  %104 = phi i64 [ 0, %137 ], [ %119, %116 ]
  %105 = icmp eq i64 %140, 0
  br i1 %105, label %112, label %106

; <label>:106:                                    ; preds = %102, %106
  %107 = phi i64 [ %109, %106 ], [ %104, %102 ]
  %108 = phi i64 [ %110, %106 ], [ %140, %102 ]
  %109 = add nuw nsw i64 %107, 1
  %110 = add i64 %108, -1
  %111 = icmp eq i64 %110, 0
  br i1 %111, label %112, label %106, !llvm.loop !11

; <label>:112:                                    ; preds = %102, %106, %134
  %113 = phi i64 [ 0, %134 ], [ %103, %102 ], [ %109, %106 ]
  %114 = load i64, i64* %72, align 8, !tbaa !4
  %115 = icmp sgt i64 %114, 0
  br i1 %115, label %157, label %182

; <label>:116:                                    ; preds = %116, %142
  %117 = phi i64 [ 0, %142 ], [ %119, %116 ]
  %118 = phi i64 [ %143, %142 ], [ %120, %116 ]
  %119 = add nuw nsw i64 %117, 8
  %120 = add i64 %118, -8
  %121 = icmp eq i64 %120, 0
  br i1 %121, label %102, label %116

; <label>:122:                                    ; preds = %122, %99
  %123 = phi i64 [ %132, %122 ], [ 0, %99 ]
  %124 = getelementptr inbounds i64, i64* %0, i64 %123
  %125 = load i64, i64* %124, align 8, !tbaa !4
  %126 = sdiv i64 %125, %101
  %127 = srem i64 %126, 10
  %128 = getelementptr inbounds [10 x i64], [10 x i64]* %4, i64 0, i64 %127
  %129 = load i64, i64* %128, align 8, !tbaa !4
  %130 = getelementptr inbounds [10 x [10 x i64]], [10 x [10 x i64]]* %3, i64 0, i64 %127, i64 %129
  store i64 %125, i64* %130, align 8, !tbaa !4
  %131 = add nsw i64 %129, 1
  store i64 %131, i64* %128, align 8, !tbaa !4
  %132 = add nuw nsw i64 %123, 1
  %133 = icmp eq i64 %132, %1
  br i1 %133, label %134, label %122

; <label>:134:                                    ; preds = %122
  %135 = load i64, i64* %69, align 16, !tbaa !4
  %136 = icmp sgt i64 %135, 0
  br i1 %136, label %137, label %112

; <label>:137:                                    ; preds = %134
  %138 = shl i64 %135, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %71, i8* nonnull align 16 %70, i64 %138, i1 false)
  %139 = add i64 %135, -1
  %140 = and i64 %135, 7
  %141 = icmp ult i64 %139, 7
  br i1 %141, label %102, label %142

; <label>:142:                                    ; preds = %137
  %143 = sub i64 %135, %140
  br label %116

; <label>:144:                                    ; preds = %58, %144
  %145 = phi i64 [ %148, %144 ], [ %59, %58 ]
  %146 = phi i64 [ %147, %144 ], [ 0, %58 ]
  %147 = add nuw nsw i64 %146, 1
  %148 = udiv i64 %145, 10
  %149 = icmp ugt i64 %145, 9
  br i1 %149, label %144, label %61

; <label>:150:                                    ; preds = %412, %63
  %151 = icmp eq i64 %64, 0
  br i1 %151, label %156, label %152

; <label>:152:                                    ; preds = %150, %152
  %153 = phi i64 [ %154, %152 ], [ %64, %150 ]
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  %154 = add i64 %153, -1
  %155 = icmp eq i64 %154, 0
  br i1 %155, label %156, label %152, !llvm.loop !12

; <label>:156:                                    ; preds = %150, %152, %408, %58
  call void @llvm.lifetime.end.p0i8(i64 80, i8* nonnull %5) #3
  call void @llvm.lifetime.end.p0i8(i64 800, i8* nonnull %6) #3
  ret void

; <label>:157:                                    ; preds = %112
  %158 = getelementptr i64, i64* %0, i64 %113
  %159 = bitcast i64* %158 to i8*
  %160 = shl i64 %114, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %159, i8* nonnull align 16 %74, i64 %160, i1 false)
  %161 = add i64 %114, -1
  %162 = and i64 %114, 7
  %163 = icmp ult i64 %161, 7
  br i1 %163, label %172, label %164

; <label>:164:                                    ; preds = %157
  %165 = sub i64 %114, %162
  br label %166

; <label>:166:                                    ; preds = %166, %164
  %167 = phi i64 [ %113, %164 ], [ %169, %166 ]
  %168 = phi i64 [ %165, %164 ], [ %170, %166 ]
  %169 = add nsw i64 %167, 8
  %170 = add i64 %168, -8
  %171 = icmp eq i64 %170, 0
  br i1 %171, label %172, label %166

; <label>:172:                                    ; preds = %166, %157
  %173 = phi i64 [ undef, %157 ], [ %169, %166 ]
  %174 = phi i64 [ %113, %157 ], [ %169, %166 ]
  %175 = icmp eq i64 %162, 0
  br i1 %175, label %182, label %176

; <label>:176:                                    ; preds = %172, %176
  %177 = phi i64 [ %179, %176 ], [ %174, %172 ]
  %178 = phi i64 [ %180, %176 ], [ %162, %172 ]
  %179 = add nsw i64 %177, 1
  %180 = add i64 %178, -1
  %181 = icmp eq i64 %180, 0
  br i1 %181, label %182, label %176, !llvm.loop !13

; <label>:182:                                    ; preds = %172, %176, %112
  %183 = phi i64 [ %113, %112 ], [ %173, %172 ], [ %179, %176 ]
  %184 = load i64, i64* %75, align 16, !tbaa !4
  %185 = icmp sgt i64 %184, 0
  br i1 %185, label %186, label %211

; <label>:186:                                    ; preds = %182
  %187 = getelementptr i64, i64* %0, i64 %183
  %188 = bitcast i64* %187 to i8*
  %189 = shl i64 %184, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %188, i8* nonnull align 16 %77, i64 %189, i1 false)
  %190 = add i64 %184, -1
  %191 = and i64 %184, 7
  %192 = icmp ult i64 %190, 7
  br i1 %192, label %201, label %193

; <label>:193:                                    ; preds = %186
  %194 = sub i64 %184, %191
  br label %195

; <label>:195:                                    ; preds = %195, %193
  %196 = phi i64 [ %183, %193 ], [ %198, %195 ]
  %197 = phi i64 [ %194, %193 ], [ %199, %195 ]
  %198 = add nsw i64 %196, 8
  %199 = add i64 %197, -8
  %200 = icmp eq i64 %199, 0
  br i1 %200, label %201, label %195

; <label>:201:                                    ; preds = %195, %186
  %202 = phi i64 [ undef, %186 ], [ %198, %195 ]
  %203 = phi i64 [ %183, %186 ], [ %198, %195 ]
  %204 = icmp eq i64 %191, 0
  br i1 %204, label %211, label %205

; <label>:205:                                    ; preds = %201, %205
  %206 = phi i64 [ %208, %205 ], [ %203, %201 ]
  %207 = phi i64 [ %209, %205 ], [ %191, %201 ]
  %208 = add nsw i64 %206, 1
  %209 = add i64 %207, -1
  %210 = icmp eq i64 %209, 0
  br i1 %210, label %211, label %205, !llvm.loop !14

; <label>:211:                                    ; preds = %201, %205, %182
  %212 = phi i64 [ %183, %182 ], [ %202, %201 ], [ %208, %205 ]
  %213 = load i64, i64* %78, align 8, !tbaa !4
  %214 = icmp sgt i64 %213, 0
  br i1 %214, label %215, label %240

; <label>:215:                                    ; preds = %211
  %216 = getelementptr i64, i64* %0, i64 %212
  %217 = bitcast i64* %216 to i8*
  %218 = shl i64 %213, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %217, i8* nonnull align 16 %80, i64 %218, i1 false)
  %219 = add i64 %213, -1
  %220 = and i64 %213, 7
  %221 = icmp ult i64 %219, 7
  br i1 %221, label %230, label %222

; <label>:222:                                    ; preds = %215
  %223 = sub i64 %213, %220
  br label %224

; <label>:224:                                    ; preds = %224, %222
  %225 = phi i64 [ %212, %222 ], [ %227, %224 ]
  %226 = phi i64 [ %223, %222 ], [ %228, %224 ]
  %227 = add nsw i64 %225, 8
  %228 = add i64 %226, -8
  %229 = icmp eq i64 %228, 0
  br i1 %229, label %230, label %224

; <label>:230:                                    ; preds = %224, %215
  %231 = phi i64 [ undef, %215 ], [ %227, %224 ]
  %232 = phi i64 [ %212, %215 ], [ %227, %224 ]
  %233 = icmp eq i64 %220, 0
  br i1 %233, label %240, label %234

; <label>:234:                                    ; preds = %230, %234
  %235 = phi i64 [ %237, %234 ], [ %232, %230 ]
  %236 = phi i64 [ %238, %234 ], [ %220, %230 ]
  %237 = add nsw i64 %235, 1
  %238 = add i64 %236, -1
  %239 = icmp eq i64 %238, 0
  br i1 %239, label %240, label %234, !llvm.loop !15

; <label>:240:                                    ; preds = %230, %234, %211
  %241 = phi i64 [ %212, %211 ], [ %231, %230 ], [ %237, %234 ]
  %242 = load i64, i64* %81, align 16, !tbaa !4
  %243 = icmp sgt i64 %242, 0
  br i1 %243, label %244, label %269

; <label>:244:                                    ; preds = %240
  %245 = getelementptr i64, i64* %0, i64 %241
  %246 = bitcast i64* %245 to i8*
  %247 = shl i64 %242, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %246, i8* nonnull align 16 %83, i64 %247, i1 false)
  %248 = add i64 %242, -1
  %249 = and i64 %242, 7
  %250 = icmp ult i64 %248, 7
  br i1 %250, label %259, label %251

; <label>:251:                                    ; preds = %244
  %252 = sub i64 %242, %249
  br label %253

; <label>:253:                                    ; preds = %253, %251
  %254 = phi i64 [ %241, %251 ], [ %256, %253 ]
  %255 = phi i64 [ %252, %251 ], [ %257, %253 ]
  %256 = add nsw i64 %254, 8
  %257 = add i64 %255, -8
  %258 = icmp eq i64 %257, 0
  br i1 %258, label %259, label %253

; <label>:259:                                    ; preds = %253, %244
  %260 = phi i64 [ undef, %244 ], [ %256, %253 ]
  %261 = phi i64 [ %241, %244 ], [ %256, %253 ]
  %262 = icmp eq i64 %249, 0
  br i1 %262, label %269, label %263

; <label>:263:                                    ; preds = %259, %263
  %264 = phi i64 [ %266, %263 ], [ %261, %259 ]
  %265 = phi i64 [ %267, %263 ], [ %249, %259 ]
  %266 = add nsw i64 %264, 1
  %267 = add i64 %265, -1
  %268 = icmp eq i64 %267, 0
  br i1 %268, label %269, label %263, !llvm.loop !16

; <label>:269:                                    ; preds = %259, %263, %240
  %270 = phi i64 [ %241, %240 ], [ %260, %259 ], [ %266, %263 ]
  %271 = load i64, i64* %84, align 8, !tbaa !4
  %272 = icmp sgt i64 %271, 0
  br i1 %272, label %273, label %298

; <label>:273:                                    ; preds = %269
  %274 = getelementptr i64, i64* %0, i64 %270
  %275 = bitcast i64* %274 to i8*
  %276 = shl i64 %271, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %275, i8* nonnull align 16 %86, i64 %276, i1 false)
  %277 = add i64 %271, -1
  %278 = and i64 %271, 7
  %279 = icmp ult i64 %277, 7
  br i1 %279, label %288, label %280

; <label>:280:                                    ; preds = %273
  %281 = sub i64 %271, %278
  br label %282

; <label>:282:                                    ; preds = %282, %280
  %283 = phi i64 [ %270, %280 ], [ %285, %282 ]
  %284 = phi i64 [ %281, %280 ], [ %286, %282 ]
  %285 = add nsw i64 %283, 8
  %286 = add i64 %284, -8
  %287 = icmp eq i64 %286, 0
  br i1 %287, label %288, label %282

; <label>:288:                                    ; preds = %282, %273
  %289 = phi i64 [ undef, %273 ], [ %285, %282 ]
  %290 = phi i64 [ %270, %273 ], [ %285, %282 ]
  %291 = icmp eq i64 %278, 0
  br i1 %291, label %298, label %292

; <label>:292:                                    ; preds = %288, %292
  %293 = phi i64 [ %295, %292 ], [ %290, %288 ]
  %294 = phi i64 [ %296, %292 ], [ %278, %288 ]
  %295 = add nsw i64 %293, 1
  %296 = add i64 %294, -1
  %297 = icmp eq i64 %296, 0
  br i1 %297, label %298, label %292, !llvm.loop !17

; <label>:298:                                    ; preds = %288, %292, %269
  %299 = phi i64 [ %270, %269 ], [ %289, %288 ], [ %295, %292 ]
  %300 = load i64, i64* %87, align 16, !tbaa !4
  %301 = icmp sgt i64 %300, 0
  br i1 %301, label %302, label %327

; <label>:302:                                    ; preds = %298
  %303 = getelementptr i64, i64* %0, i64 %299
  %304 = bitcast i64* %303 to i8*
  %305 = shl i64 %300, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %304, i8* nonnull align 16 %89, i64 %305, i1 false)
  %306 = add i64 %300, -1
  %307 = and i64 %300, 7
  %308 = icmp ult i64 %306, 7
  br i1 %308, label %317, label %309

; <label>:309:                                    ; preds = %302
  %310 = sub i64 %300, %307
  br label %311

; <label>:311:                                    ; preds = %311, %309
  %312 = phi i64 [ %299, %309 ], [ %314, %311 ]
  %313 = phi i64 [ %310, %309 ], [ %315, %311 ]
  %314 = add nsw i64 %312, 8
  %315 = add i64 %313, -8
  %316 = icmp eq i64 %315, 0
  br i1 %316, label %317, label %311

; <label>:317:                                    ; preds = %311, %302
  %318 = phi i64 [ undef, %302 ], [ %314, %311 ]
  %319 = phi i64 [ %299, %302 ], [ %314, %311 ]
  %320 = icmp eq i64 %307, 0
  br i1 %320, label %327, label %321

; <label>:321:                                    ; preds = %317, %321
  %322 = phi i64 [ %324, %321 ], [ %319, %317 ]
  %323 = phi i64 [ %325, %321 ], [ %307, %317 ]
  %324 = add nsw i64 %322, 1
  %325 = add i64 %323, -1
  %326 = icmp eq i64 %325, 0
  br i1 %326, label %327, label %321, !llvm.loop !18

; <label>:327:                                    ; preds = %317, %321, %298
  %328 = phi i64 [ %299, %298 ], [ %318, %317 ], [ %324, %321 ]
  %329 = load i64, i64* %90, align 8, !tbaa !4
  %330 = icmp sgt i64 %329, 0
  br i1 %330, label %331, label %356

; <label>:331:                                    ; preds = %327
  %332 = getelementptr i64, i64* %0, i64 %328
  %333 = bitcast i64* %332 to i8*
  %334 = shl i64 %329, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %333, i8* nonnull align 16 %92, i64 %334, i1 false)
  %335 = add i64 %329, -1
  %336 = and i64 %329, 7
  %337 = icmp ult i64 %335, 7
  br i1 %337, label %346, label %338

; <label>:338:                                    ; preds = %331
  %339 = sub i64 %329, %336
  br label %340

; <label>:340:                                    ; preds = %340, %338
  %341 = phi i64 [ %328, %338 ], [ %343, %340 ]
  %342 = phi i64 [ %339, %338 ], [ %344, %340 ]
  %343 = add nsw i64 %341, 8
  %344 = add i64 %342, -8
  %345 = icmp eq i64 %344, 0
  br i1 %345, label %346, label %340

; <label>:346:                                    ; preds = %340, %331
  %347 = phi i64 [ undef, %331 ], [ %343, %340 ]
  %348 = phi i64 [ %328, %331 ], [ %343, %340 ]
  %349 = icmp eq i64 %336, 0
  br i1 %349, label %356, label %350

; <label>:350:                                    ; preds = %346, %350
  %351 = phi i64 [ %353, %350 ], [ %348, %346 ]
  %352 = phi i64 [ %354, %350 ], [ %336, %346 ]
  %353 = add nsw i64 %351, 1
  %354 = add i64 %352, -1
  %355 = icmp eq i64 %354, 0
  br i1 %355, label %356, label %350, !llvm.loop !19

; <label>:356:                                    ; preds = %346, %350, %327
  %357 = phi i64 [ %328, %327 ], [ %347, %346 ], [ %353, %350 ]
  %358 = load i64, i64* %93, align 16, !tbaa !4
  %359 = icmp sgt i64 %358, 0
  br i1 %359, label %360, label %385

; <label>:360:                                    ; preds = %356
  %361 = getelementptr i64, i64* %0, i64 %357
  %362 = bitcast i64* %361 to i8*
  %363 = shl i64 %358, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %362, i8* nonnull align 16 %95, i64 %363, i1 false)
  %364 = add i64 %358, -1
  %365 = and i64 %358, 7
  %366 = icmp ult i64 %364, 7
  br i1 %366, label %375, label %367

; <label>:367:                                    ; preds = %360
  %368 = sub i64 %358, %365
  br label %369

; <label>:369:                                    ; preds = %369, %367
  %370 = phi i64 [ %357, %367 ], [ %372, %369 ]
  %371 = phi i64 [ %368, %367 ], [ %373, %369 ]
  %372 = add nsw i64 %370, 8
  %373 = add i64 %371, -8
  %374 = icmp eq i64 %373, 0
  br i1 %374, label %375, label %369

; <label>:375:                                    ; preds = %369, %360
  %376 = phi i64 [ undef, %360 ], [ %372, %369 ]
  %377 = phi i64 [ %357, %360 ], [ %372, %369 ]
  %378 = icmp eq i64 %365, 0
  br i1 %378, label %385, label %379

; <label>:379:                                    ; preds = %375, %379
  %380 = phi i64 [ %382, %379 ], [ %377, %375 ]
  %381 = phi i64 [ %383, %379 ], [ %365, %375 ]
  %382 = add nsw i64 %380, 1
  %383 = add i64 %381, -1
  %384 = icmp eq i64 %383, 0
  br i1 %384, label %385, label %379, !llvm.loop !20

; <label>:385:                                    ; preds = %375, %379, %356
  %386 = phi i64 [ %357, %356 ], [ %376, %375 ], [ %382, %379 ]
  %387 = load i64, i64* %96, align 8, !tbaa !4
  %388 = icmp sgt i64 %387, 0
  br i1 %388, label %389, label %408

; <label>:389:                                    ; preds = %385
  %390 = getelementptr i64, i64* %0, i64 %386
  %391 = bitcast i64* %390 to i8*
  %392 = shl i64 %387, 3
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %391, i8* nonnull align 16 %98, i64 %392, i1 false)
  %393 = add i64 %387, -1
  %394 = and i64 %387, 7
  %395 = icmp ult i64 %393, 7
  br i1 %395, label %402, label %396

; <label>:396:                                    ; preds = %389
  %397 = sub i64 %387, %394
  br label %398

; <label>:398:                                    ; preds = %398, %396
  %399 = phi i64 [ %397, %396 ], [ %400, %398 ]
  %400 = add i64 %399, -8
  %401 = icmp eq i64 %400, 0
  br i1 %401, label %402, label %398

; <label>:402:                                    ; preds = %398, %389
  %403 = icmp eq i64 %394, 0
  br i1 %403, label %408, label %404

; <label>:404:                                    ; preds = %402, %404
  %405 = phi i64 [ %406, %404 ], [ %394, %402 ]
  %406 = add i64 %405, -1
  %407 = icmp eq i64 %406, 0
  br i1 %407, label %408, label %404, !llvm.loop !21

; <label>:408:                                    ; preds = %402, %404, %385
  %409 = mul nsw i64 %101, 10
  %410 = add nuw nsw i64 %100, 1
  %411 = icmp eq i64 %410, %147
  br i1 %411, label %156, label %99

; <label>:412:                                    ; preds = %412, %66
  %413 = phi i64 [ %67, %66 ], [ %414, %412 ]
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  call void @llvm.memset.p0i8.i64(i8* nonnull align 16 %5, i8 0, i64 80, i1 false)
  %414 = add i64 %413, -8
  %415 = icmp eq i64 %414, 0
  br i1 %415, label %150, label %412
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #1

attributes #0 = { norecurse nounwind readonly sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="-3dnow,-3dnowa,-aes,-avx,-avx2,-avx512bitalg,-avx512bw,-avx512cd,-avx512dq,-avx512er,-avx512f,-avx512ifma,-avx512pf,-avx512vbmi,-avx512vbmi2,-avx512vl,-avx512vnni,-avx512vpopcntdq,-f16c,-fma,-fma4,-fxsr,-gfni,-mmx,-pclmul,-sha,-sse,-sse2,-sse3,-sse4.1,-sse4.2,-sse4a,-ssse3,-vaes,-vpclmulqdq,-x87,-xop,-xsave,-xsaveopt" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
!4 = !{!5, !5, i64 0}
!5 = !{!"long", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.unroll.disable"}
!10 = distinct !{!10, !9}
!11 = distinct !{!11, !9}
!12 = distinct !{!12, !9}
!13 = distinct !{!13, !9}
!14 = distinct !{!14, !9}
!15 = distinct !{!15, !9}
!16 = distinct !{!16, !9}
!17 = distinct !{!17, !9}
!18 = distinct !{!18, !9}
!19 = distinct !{!19, !9}
!20 = distinct !{!20, !9}
!21 = distinct !{!21, !9}
