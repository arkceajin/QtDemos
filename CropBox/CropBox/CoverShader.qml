import QtQuick 2.12

ShaderEffect {
    property rect sourceRect: cover.sourceRect
    property real xStep: 1 / cover.width
    property real yStep: 1 / cover.height

    fragmentShader: "
        uniform lowp sampler2D source;
        varying mediump vec2 qt_TexCoord0;
        uniform lowp float qt_Opacity;

        uniform highp vec4 sourceRect;

        uniform highp float xStep;
        uniform highp float yStep;

        bool insideBox(vec2 topLeft, vec2 bottomRight, vec2 point){
            vec2 s = step(topLeft, point) - step(bottomRight, point);
            return (s.x * s.y) > 0.0;
        }

        void main() {
            vec2 topLeft = vec2(sourceRect.x*xStep, sourceRect.y*yStep);
            vec2 bottomRight = topLeft + vec2(sourceRect.z*xStep, sourceRect.w*yStep);
            gl_FragColor = texture2D(source, qt_TexCoord0) * qt_Opacity;
            if(insideBox(topLeft, bottomRight, qt_TexCoord0))
                gl_FragColor *= 0.0;
        }
    "
}
