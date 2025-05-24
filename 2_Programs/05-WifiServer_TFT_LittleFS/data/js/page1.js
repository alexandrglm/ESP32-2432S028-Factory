(() => {
  const canvas = document.getElementById("arkanoid");
  const ctx = canvas.getContext("2d");

  const brickConfig = {
    rows: 3, cols: 5,
    width: 75, height: 20,
    padding: 10,
    offsetTop: 30, offsetLeft: 35,
  };

  let ball, paddle, bricks, gameOver;

  const resetBtn = {
    x: 0, y: 0, width: 140, height: 36,
    hover: false
  };

  function init() {
    ball = { x: canvas.width / 2, y: canvas.height / 2, dx: 3, dy: -3, radius: 6 };
    paddle = { width: 75, height: 10, x: (canvas.width - 75) / 2, speed: 6, right: false, left: false };
    gameOver = false;
    bricks = [];

    for (let c = 0; c < brickConfig.cols; c++) {
      bricks[c] = [];
      for (let r = 0; r < brickConfig.rows; r++) {
        bricks[c][r] = {
          x: 0, y: 0, status: 1,
          color: '#' + Math.floor(Math.random() * 0xFFFFFF).toString(16).padStart(6, '0')
        };
      }
    }
  }

  function drawBall() {
    ctx.beginPath();
    ctx.arc(ball.x, ball.y, ball.radius, 0, Math.PI * 2);
    ctx.fillStyle = "#f00";
    ctx.fill();
    ctx.closePath();
  }

  function drawPaddle() {
    ctx.beginPath();
    ctx.rect(paddle.x, canvas.height - paddle.height - 5, paddle.width, paddle.height);
    ctx.fillStyle = "#0f0";
    ctx.fill();
    ctx.closePath();
  }

  function drawBricks() {
    for (let c = 0; c < brickConfig.cols; c++) {
      for (let r = 0; r < brickConfig.rows; r++) {
        if (bricks[c][r].status) {
          const bX = c * (brickConfig.width + brickConfig.padding) + brickConfig.offsetLeft;
          const bY = r * (brickConfig.height + brickConfig.padding) + brickConfig.offsetTop;
          bricks[c][r].x = bX;
          bricks[c][r].y = bY;
          ctx.beginPath();
          ctx.rect(bX, bY, brickConfig.width, brickConfig.height);
          ctx.fillStyle = bricks[c][r].color;
          ctx.fill();
          ctx.closePath();
        }
      }
    }
  }

  function collision() {
    for (let c = 0; c < brickConfig.cols; c++) {
      for (let r = 0; r < brickConfig.rows; r++) {
        const b = bricks[c][r];
        if (
          b.status &&
          ball.x > b.x &&
          ball.x < b.x + brickConfig.width &&
          ball.y > b.y &&
          ball.y < b.y + brickConfig.height
        ) {
          ball.dy = -ball.dy;
          b.status = 0;
        }
      }
    }
  }

  function drawGameOver() {
    const text = "GAME OVER";
    ctx.fillStyle = 'white';
    ctx.font = 'bold 32px Arial';
    ctx.textAlign = 'center';
    ctx.fillText(text, canvas.width / 2, canvas.height / 2 - 20);

    resetBtn.x = (canvas.width - resetBtn.width) / 2;
    resetBtn.y = (canvas.height / 2) + 10;

    ctx.fillStyle = resetBtn.hover ? '#09f' : '#004080';
    ctx.strokeStyle = resetBtn.hover ? '#0af' : '#002050';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.roundRect(resetBtn.x, resetBtn.y, resetBtn.width, resetBtn.height, 8);
    ctx.fill();
    ctx.stroke();
    ctx.closePath();

    ctx.fillStyle = 'white';
    ctx.font = 'bold 18px Arial';
    ctx.fillText('Play Again!', canvas.width / 2, resetBtn.y + resetBtn.height / 2 + 6);
  }

  function isInResetBtn(x, y) {
    return (
      x >= resetBtn.x &&
      x <= resetBtn.x + resetBtn.width &&
      y >= resetBtn.y &&
      y <= resetBtn.y + resetBtn.height
    );
  }

  function draw() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    drawBricks();
    drawBall();
    drawPaddle();
    collision();

    if (ball.x + ball.dx > canvas.width - ball.radius || ball.x + ball.dx < ball.radius)
      ball.dx = -ball.dx;

    if (ball.y + ball.dy < ball.radius) {
      ball.dy = -ball.dy;
    } else if (ball.y + ball.dy > canvas.height - ball.radius - paddle.height - 5) {
      if (ball.x > paddle.x && ball.x < paddle.x + paddle.width) {
        ball.dy = -ball.dy;
      } else {
        gameOver = true;
      }
    }

    if (paddle.right && paddle.x < canvas.width - paddle.width) paddle.x += paddle.speed;
    if (paddle.left && paddle.x > 0) paddle.x -= paddle.speed;

    ball.x += ball.dx;
    ball.y += ball.dy;

    if (gameOver) {
      drawGameOver();
    } else {
      requestAnimationFrame(draw);
    }
  }

  // Paddle -> Key
  document.addEventListener("keydown", e => {
    if (!gameOver) {
      if (e.key === "ArrowRight") paddle.right = true;
      else if (e.key === "ArrowLeft") paddle.left = true;
    }
  });
  document.addEventListener("keyup", e => {
    if (!gameOver) {
      if (e.key === "ArrowRight") paddle.right = false;
      else if (e.key === "ArrowLeft") paddle.left = false;
    }
  });

  // PADDLE -> Mpuse
  canvas.addEventListener("mousemove", e => {
    if (!gameOver) {
      const rect = canvas.getBoundingClientRect();
      const mouseX = e.clientX - rect.left;
      paddle.x = mouseX - paddle.width / 2;
      if (paddle.x < 0) paddle.x = 0;
      if (paddle.x > canvas.width - paddle.width) paddle.x = canvas.width - paddle.width;
    }
  });

  // PADDLE -> Touch
  canvas.addEventListener("touchmove", e => {
    if (!gameOver) {
      e.preventDefault();
      const rect = canvas.getBoundingClientRect();
      const touchX = e.touches[0].clientX - rect.left;
      paddle.x = touchX - paddle.width / 2;
      if (paddle.x < 0) paddle.x = 0;
      if (paddle.x > canvas.width - paddle.width) paddle.x = canvas.width - paddle.width;
    }
  }, { passive: false });


  canvas.addEventListener("mousemove", e => {
    if (gameOver) {
      const rect = canvas.getBoundingClientRect();
      const x = e.clientX - rect.left;
      const y = e.clientY - rect.top;
      resetBtn.hover = isInResetBtn(x, y);
      drawGameOver();
    }
  });

  canvas.addEventListener("click", e => {
    if (gameOver) {
      const rect = canvas.getBoundingClientRect();
      const x = e.clientX - rect.left;
      const y = e.clientY - rect.top;
      if (isInResetBtn(x, y)) {
        init();
        draw();
      }
    }
  });

  if (!CanvasRenderingContext2D.prototype.roundRect) {
    CanvasRenderingContext2D.prototype.roundRect = function (x, y, w, h, r) {
      if (w < 2 * r) r = w / 2;
      if (h < 2 * r) r = h / 2;
      this.beginPath();
      this.moveTo(x + r, y);
      this.arcTo(x + w, y, x + w, y + h, r);
      this.arcTo(x + w, y + h, x, y + h, r);
      this.arcTo(x, y + h, x, y, r);
      this.arcTo(x, y, x + w, y, r);
      this.closePath();
      return this;
    }
  }

  init();
  draw();
})();