"use strict";
let gl;
let programInfo;
let quad;

async function main() {
    const canvas = document.getElementById('ourCanvas');
    gl = canvas.getContext('webgl');
    console.log(gl);

    await createShaders();

    createQuad();

    drawQuad();
}

let t = 1.1;
function drawQuad() {
    t = Math.pow(t, 1.005);
    gl.useProgram(programInfo.program);
    gl.bindBuffer(gl.ARRAY_BUFFER, quad.position);
    gl.vertexAttribPointer(programInfo.attribLocations.vertexPosition, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(programInfo.attribLocations.vertexPosition);
    const location = gl.getUniformLocation(programInfo.program, 'zoom');
    gl.uniform1f(location, t);
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

    requestAnimationFrame(drawQuad);
}

function createQuad() {
    quad = initBuffers();
}

function initBuffers() {
    const positionBuffer = gl.createBuffer();

    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

    const positions = [
        1.0, 1.0,
        -1.0, 1.0,
        1.0, -1.0,
        -1.0, -1.0,
    ];

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

    return {
        position: positionBuffer,
    };
}

async function createShaders() {
    const vsSource = `
    attribute vec2 aVertexPosition;

    void main() {
      gl_Position = vec4(aVertexPosition, 0.0, 1.0);
    }
  `;

    const fsSource = await getFragmentShaderSource();

    const shaderProgram = initShaderProgram(gl, vsSource, fsSource);

    programInfo = {
        program: shaderProgram,
        attribLocations: {
            vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
        }
    }
}

function initShaderProgram(gl, vsSource, fsSource) {
    const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
    const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

    const shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        alert(`Unable to initialize the shader program: ${gl.getProgramInfoLog(shaderProgram)}`);
        return null;
    }

    return shaderProgram;
}

function loadShader(gl, type, source) {
    const shader = gl.createShader(type);

    gl.shaderSource(shader, source);

    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(`An error occurred compiling the shaders: ${gl.getShaderInfoLog(shader)}`);
        gl.deleteShader(shader);
        return null;
    }

    return shader;
}

async function getFragmentShaderSource() {
    const response = await fetch('./fragment.glsl');
    const fsSource = await response.text();
    return fsSource;
}

main();