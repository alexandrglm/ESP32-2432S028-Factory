const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer({ canvas: document.getElementById("webgl-canvas") });
renderer.setSize(window.innerWidth, window.innerHeight);

// document.body.appendChild(renderer.domElement);

const geometry = new THREE.PlaneGeometry(2, 2);

const vertexShader = `
void main() {
    gl_Position = vec4(position, 1.0);
}
`;

const fragmentShader = `
precision highp float;

uniform vec2 iResolution;
uniform float iTime;

float grid(vec2 uv, float battery) {
    vec2 size = vec2(uv.y, uv.y * uv.y * 0.2) * 0.01;
    uv += vec2(0.0, iTime * 10.0 * (battery + 0.05));
    uv = abs(fract(uv) - 0.5);
    vec2 lines = smoothstep(size, vec2(0.0), uv);
    lines += smoothstep(size * 5.0, vec2(0.0), uv) * 0.4 * battery;
    return clamp(lines.x + lines.y, 0.0, 3.0);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = (2.0 * fragCoord.xy - iResolution.xy) / iResolution.y;
    float battery = 1.0;

    float fog = smoothstep(0.1, -0.02, abs(uv.y + 0.2));
    vec3 col = vec3(1, 0.1, 0.001);

    if (uv.y < -0.2) {
        uv.y = 3.0 / (abs(uv.y + 0.2) + 0.05);
        uv.x *= uv.y * 0.5;

        uv += vec2(0.0, iTime * 10.0 * (battery + 0.05));

        float fila = floor(uv.y);
        float columna = floor(uv.x);

        vec3 colorTesela1 = vec3(0.0, 0.0, 0.5);
        vec3 colorTesela2 = vec3(0.5, 0.0, 0.5);

        float patron = mod(fila + columna, 2.0);
        col = (patron < 1.0) ? colorTesela1 : colorTesela2;

        float gridVal = grid(uv, battery);
        col = mix(col, vec3(0.5, 1., 0.22), gridVal);
    } else {
        vec3 colorHorizonte = vec3(0.5, 1.0, 0.22);
        vec3 colorCielo = vec3(0.5, 0.0, 0.5);

        col = mix(colorHorizonte, colorCielo, smoothstep(-0.9, 1.0, uv.y));
    }

    col += fog * fog * fog;
    col = mix(vec3(col.r, col.r, col.r) * 0.5, col, battery * 0.7);

    fragColor = vec4(col, 1.0);
}

void main() {
    mainImage(gl_FragColor, gl_FragCoord.xy);
}
`;

const material = new THREE.ShaderMaterial({
    vertexShader,
    fragmentShader,
    uniforms: {
        iTime: { value: 0 },
        iResolution: { value: new THREE.Vector2(window.innerWidth, window.innerHeight) },
    }
});

const mesh = new THREE.Mesh(geometry, material);
scene.add(mesh);

camera.position.z = 1;

function animateSky(time) {
    requestAnimationFrame(animateSky);
    material.uniforms.iTime.value = time * 0.001;
    renderer.render(scene, camera);
}

animateSky();

const plane = new THREE.Mesh(geometry, material);
scene.add(plane);

camera.position.z = 1;

const zoomFactor = 1.5;
const fbWidth = window.innerWidth * zoomFactor;
const fbHeight = window.innerHeight * zoomFactor;

const renderTarget = new THREE.WebGLRenderTarget(fbWidth, fbHeight);

const fbScene = new THREE.Scene();
const fbCamera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 2.0);
fbCamera.position.z = 1;

const fbMaterial = new THREE.MeshBasicMaterial({ map: renderTarget.texture });
const fbPlane = new THREE.Mesh(geometry, fbMaterial);
fbScene.add(fbPlane);

fbPlane.scale.set(window.innerWidth / window.innerHeight, 1, 1);

let mouseX = 0;
let mouseY = 0;

window.addEventListener('mousemove', (event) => {
    mouseX = (event.clientX / window.innerWidth) * 2 - 1;
    mouseY = -(event.clientY / window.innerHeight) * 1 + 1;

    fbPlane.material.map.offset.set(-mouseX * (zoomFactor - 1) / 2, -mouseY * (zoomFactor - 1) / 2);
});

function animate() {
    requestAnimationFrame(animate);

    material.uniforms.iTime.value = performance.now() / 1000;

    renderer.setRenderTarget(renderTarget);
    renderer.render(scene, camera);

    renderer.setRenderTarget(null);
    renderer.render(fbScene, fbCamera);
}
animate();

window.addEventListener("resize", () => {
    const width = window.innerWidth;
    const height = window.innerHeight;
    renderer.setSize(width, height);
    camera.aspect = width / height;
    camera.updateProjectionMatrix();
    material.uniforms.iResolution.value.set(width, height);

    renderTarget.setSize(width * zoomFactor, height * zoomFactor);
    fbPlane.scale.set(width / height, 1, 1);
});
