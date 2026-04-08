#type vertex
#version 430

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;

uniform mat4 u_ViewProjection;

out vec3 v_LocalPosition;
out vec4 v_Color;
out float v_Thickness;
out float v_Fade;

void main()
{
	v_LocalPosition = a_LocalPosition;
	v_Color = a_Color;
	v_Thickness = a_Thickness;
	v_Fade = a_Fade;

	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 430

layout(location = 0) out vec4 finalColor;

in vec3 v_LocalPosition;
in vec4 v_Color;
in float v_Thickness;
in float v_Fade;

void main()
{
	// 计算当前像素距离圆心的距离 (在 -1 到 1 的局部坐标系中)
	float dist = length(v_LocalPosition.xy);

	// 使用 smoothstep 进行抗锯齿边缘处理
	// dist 从 (1.0 - v_Fade) 到 1.0 时，透明度平滑过度到 0
	float circleAlpha = 1.0 - smoothstep(1.0 - v_Fade, 1.0, dist);

	// 处理空心圆（如果我们传入的 thickness < 1.0）
	circleAlpha *= smoothstep(1.0 - v_Thickness - v_Fade, 1.0 - v_Thickness, dist);

	// 丢弃完全透明的像素，优化深度测试和性能
	if (circleAlpha == 0.0)
		discard;

	finalColor = v_Color;
	finalColor.a *= circleAlpha;
}