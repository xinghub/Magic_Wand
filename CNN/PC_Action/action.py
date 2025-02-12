import serial
import threading
import tkinter as tk
from math import cos, sin, radians, pi
from random import randint, uniform
from time import sleep
from colorsys import hsv_to_rgb


class AdvancedAnimator:
    def __init__(self, port='COM38', baudrate=921600):
        self.window = tk.Tk()
        self.window.title("Advanced Serial Animator")
        self.window.geometry("800x600")
        self.canvas = tk.Canvas(self.window, bg='black')
        self.canvas.pack(fill=tk.BOTH, expand=True)

        # 动画控制
        self.anim_queue = []
        self.active_effects = []
        self.particle_systems = []

        # 初始化串口
        try:
            self.ser = serial.Serial(port, baudrate, timeout=1)
        except Exception as e:
            print(f"Serial Error: {e}")
            exit()

        # 启动线程
        self.running = True
        self.thread = threading.Thread(target=self.serial_reader)
        self.thread.start()

        self.window.protocol("WM_DELETE_WINDOW", self.clean_exit)
        self.window.after(16, self.update_animations)

    # 核心动画方法 --------------------------------------------------
    def start_animation(self, num):
        self.clear_effects()
        anim_map = {
            '0': self.circular_vortex,
            '1': self.blooming_flower,
            '2': self.liquid_ripple,
            '3': self.rotating_tesseract,
            '4': self.meteor_shower,
            '5': self.particle_explosion,
            '6': self.snowfall,
            '7': self.flame_dance,
            '8': self.matrix_rain,
            '9': self.galaxy_spiral
        }
        if num in anim_map:
            anim_map[num]()

    # 0: 环形漩涡 --------------------------------------------------
    def circular_vortex(self):
        steps = 200
        for i in range(steps):
            angle = 2 * pi * i / steps
            radius = 150 * (i / steps)
            hue = i / steps
            r, g, b = [int(255 * c) for c in hsv_to_rgb(hue, 1, 1)]
            x = 400 + radius * cos(angle)
            y = 300 + radius * sin(angle)
            obj = self.canvas.create_oval(x - 3, y - 3, x + 3, y + 3,
                                          fill=f'#{r:02x}{g:02x}{b:02x}', outline='')
            self.active_effects.append(obj)
            self.canvas.tag_lower(obj)
            self.window.after(10)

    # 1: 绽放花朵 --------------------------------------------------
    def blooming_flower(self):
        petals = 12
        max_radius = 150
        for frame in range(30):
            self.canvas.delete("petal")
            current_radius = max_radius * (frame / 30)
            for i in range(petals):
                angle = 2 * pi * i / petals
                x1 = 400 + current_radius * cos(angle)
                y1 = 300 + current_radius * sin(angle)
                x2 = 400 + current_radius * cos(angle + pi / 6)
                y2 = 300 + current_radius * sin(angle + pi / 6)
                color = f'#{randint(200, 255):02x}{randint(0, 50):02x}{randint(100, 150):02x}'
                obj = self.canvas.create_line(400, 300, x1, y1, x2, y2, width=3, fill=color, tags="petal", smooth=True)
                self.active_effects.append(obj)
            self.window.update()
            self.window.after(50)

    # 2: 液体涟漪 --------------------------------------------------
    def liquid_ripple(self):
        circles = []
        for i in range(1, 15):
            alpha = 1 - i / 15
            # 将 rgba(50, 150, 255, alpha) 转换为 RGB
            # 注意：Tkinter 不支持透明度，所以只能使用 RGB 值，并可能需要调整透明度的视觉效果
            red = 50
            green = 150
            blue = 255
            # 调整颜色亮度来模拟透明度
            if alpha < 0:
                alpha = 0
            brightness = alpha  # 透明度作为亮度调节因子
            rgb_color = (int(red * brightness), int(green * brightness), int(blue * brightness))
            hex_color = "#{:02x}{:02x}{:02x}".format(rgb_color[0], rgb_color[1], rgb_color[2])
            obj = self.canvas.create_oval(400 - 20 * i, 300 - 20 * i,
                                          400 + 20 * i, 300 + 20 * i,
                                          outline=hex_color, width=3)
            circles.append(obj)
            self.active_effects.extend(circles)
            self.window.after(100)
            self.window.update()
        for circle in circles:
            self.canvas.delete(circle)

    # 3: 旋转超立方体 ----------------------------------------------
    def rotating_tesseract(self):
        vertices = [(cos(angle), sin(angle)) for angle in [0, pi / 2, pi, 3 * pi / 2]]
        connections = [(0, 1), (1, 2), (2, 3), (3, 0)]
        for _ in range(100):
            self.canvas.delete("cube")
            time = _ / 10
            for i, (x, y) in enumerate(vertices):
                rot_x = x * cos(time) - y * sin(time)
                rot_y = x * sin(time) + y * cos(time)
                scale = 80 + 20 * sin(time * 2)
                px = 400 + rot_x * scale
                py = 300 + rot_y * scale
                self.canvas.create_oval(px - 5, py - 5, px + 5, py + 5,
                                        fill="#FF69B4", tags="cube")
                if i > 0:
                    self.canvas.create_line(prev_px, prev_py, px, py,
                                            fill="#FF1493", width=2, tags="cube")
                prev_px, prev_py = px, py
            self.window.update()
            self.window.after(30)

    # 4: 流星雨 ----------------------------------------------------
    def meteor_shower(self):
        class Meteor:
            def __init__(self, canvas):
                self.canvas = canvas
                self.x = randint(100, 700)
                self.y = -20
                self.speed = uniform(3, 7)
                self.tail = []
                self.body = canvas.create_oval(self.x - 3, self.y - 3,
                                               self.x + 3, self.y + 3, fill='white')

            def update(self):
                self.y += self.speed
                self.x += uniform(-1, 1)
                self.canvas.move(self.body, 0, self.speed)
                # 添加拖尾
                self.tail.append(self.canvas.create_oval(
                    self.x - 2, self.y - 2, self.x + 2, self.y + 2,
                    fill=f'rgba(255,255,255,{0.5})'))
                if len(self.tail) > 5:
                    self.canvas.delete(self.tail.pop(0))
                return self.y < 620

        meteors = [Meteor(self.canvas) for _ in range(10)]
        for _ in range(50):
            for meteor in meteors[:]:
                if not meteor.update():
                    meteors.remove(meteor)
            self.window.update()
            self.window.after(30)

    # 5: 粒子爆炸 --------------------------------------------------
    def particle_explosion(self):
        particles = []
        for _ in range(200):
            angle = uniform(0, 2 * pi)
            speed = uniform(1, 5)
            life = 50
            color = f'#{randint(200, 255):02x}{randint(0, 100):02x}00'
            x, y = 400, 300
            particles.append((x, y, angle, speed, life, color))

        def update():
            new_particles = []
            for p in particles:
                x, y, angle, speed, life, color = p
                dx = cos(angle) * speed
                dy = sin(angle) * speed
                obj = self.canvas.create_oval(x - 2, y - 2, x + 2, y + 2, fill=color)
                self.active_effects.append(obj)
                new_particles.append((
                    x + dx, y + dy, angle, speed * 0.95, life - 1, color
                ))
            return new_particles

        for _ in range(50):
            particles = update()
            self.window.update()
            self.window.after(30)

    # 其他动画方法（限于篇幅展示部分，完整版包含所有数字）-----------
    # 5: 粒子爆炸 --------------------------------------------------
    def snowfall(self):
        particles = []
        # 增加粒子数量，让爆炸更密集
        for _ in range(500):
            angle = uniform(0, 2 * pi)
            speed = uniform(2, 10)  # 增加粒子初始速度
            life = 100  # 增加粒子生命周期
            color = f'#{randint(100, 255):02x}{randint(100, 255):02x}{randint(100, 255):02x}'
            x, y = 400, 300
            particles.append({
                'x': x,
                'y': y,
                'angle': angle,
                'speed': speed,
                'life': life,
                'color': color
            })

        def update():
            new_particles = []
            for p in particles:
                dx = cos(p['angle']) * p['speed']
                dy = sin(p['angle']) * p['speed']
                p['x'] += dx
                p['y'] += dy
                p['life'] -= 1
                # 为粒子添加重力和衰减效果
                p['speed'] *= 0.95
                p['speed'] += 0.2
                # 绘制粒子
                size = 5  # 增大粒子大小
                obj = self.canvas.create_oval(
                    p['x'] - size, p['y'] - size,
                    p['x'] + size, p['y'] + size,
                    fill=p['color']
                )
                self.active_effects.append(obj)
                # 添加随机晃动效果
                p['angle'] += uniform(-0.1, 0.1)
                if p['life'] > 0:
                    new_particles.append(p)
            return new_particles

        for _ in range(200):  # 延长爆炸时间
            particles = update()
            self.window.update()
            self.window.after(15)

    # 6: 火焰舞动感 --------------------------------------------------
    def flame_dance(self):
        flames = []
        for _ in range(50):
            x = randint(100, 700)
            y = randint(300, 600)
            size = randint(10, 30)
            color = f'#{randint(200, 255):02x}{randint(100, 150):02x}00'
            flame = {
                'x': x,
                'y': y,
                'size': size,
                'color': color,
                'life': randint(50, 100)
            }
            flames.append(flame)

        def update_flames():
            new_flames = []
            for f in flames:
                f['y'] -= 2  # 增加火焰上升速度
                f['size'] -= 0.5
                f['life'] -= 1
                # 为火焰添加随机摆动效果
                f['x'] += randint(-3, 3)
                f['y'] += randint(-1, 1)
                # 绘制火焰
                obj = self.canvas.create_oval(
                    f['x'] - f['size'], f['y'] - f['size'],
                    f['x'] + f['size'], f['y'] + f['size'],
                    fill=f['color'], outline=f['color']
                )
                self.active_effects.append(obj)
                if f['life'] > 0:
                    new_flames.append(f)
            return new_flames

        for _ in range(200):
            flames = update_flames()
            self.window.update()
            self.window.after(20)

    # 11: 旋转超立方体 ----------------------------------------------
    def matrix_rain(self):
        vertices = [(cos(angle), sin(angle)) for angle in
                    [0, pi / 4, pi / 2, 3 * pi / 4, pi, 5 * pi / 4, 3 * pi / 2, 7 * pi / 4]]
        connections = [(0, 1), (1, 2), (2, 3), (3, 0), (4, 5), (5, 6), (6, 7), (7, 4),
                       (0, 4), (1, 5), (2, 6), (3, 7)]
        for _ in range(200):
            self.canvas.delete("cube")
            time = _ / 10
            prev_px, prev_py = 400, 300
            for i, (x, y) in enumerate(vertices):
                rot_x = x * cos(time) - y * sin(time)
                rot_y = x * sin(time) + y * cos(time)
                scale = 100 + 50 * sin(time * 2)
                px = 400 + rot_x * scale
                py = 300 + rot_y * scale
                self.canvas.create_oval(px - 10, py - 10, px + 10, py + 10, fill="#FF69B4", tags="cube")
                if i > 0:
                    self.canvas.create_line(prev_px, prev_py, px, py, fill="#FF1493", width=5, tags="cube")
                prev_px, prev_py = px, py
                for j, (a, b) in enumerate(connections):
                    if i == a:
                        xa = rot_x * scale
                        ya = rot_y * scale
                        xb = vertices[b][0] * cos(time) - vertices[b][1] * sin(time)
                        yb = vertices[b][0] * sin(time) + vertices[b][1] * cos(time)
                        xb *= scale
                        yb *= scale
                        self.canvas.create_line(
                            400 + xa, 300 + ya,
                            400 + xb, 300 + yb,
                            fill="#AA00FF",
                            width=3,
                            tags="cube"
                        )
            self.window.update()
            self.window.after(20)

    # 12: 流星雨 ----------------------------------------------------
    def galaxy_spiral(self):
        meteors = []
        for _ in range(30):
            meteors.append({
                'x': randint(100, 700),
                'y': randint(-100, 0),
                'speed': uniform(5, 15),
                'color': f'#{randint(100, 255):02x}{randint(100, 255):02x}{randint(100, 255):02x}',
                'tail': []
            })

        def update_meteors():
            new_meteors = []
            for meteor in meteors:
                meteor['y'] += meteor['speed']
                meteor['x'] += uniform(-3, 3)
                # 绘制流星主体
                obj = self.canvas.create_oval(
                    meteor['x'] - 5, meteor['y'] - 5,
                    meteor['x'] + 5, meteor['y'] + 5,
                    fill=meteor['color']
                )
                self.active_effects.append(obj)
                # 添加拖尾
                meteor['tail'].append(obj)
                if len(meteor['tail']) > 20:
                    self.canvas.delete(meteor['tail'].pop(0))
                if meteor['y'] < 620:
                    new_meteors.append(meteor)
            return new_meteors

        for _ in range(200):
            meteors = update_meteors()
            self.window.update()
            self.window.after(20)

    def clear_effects(self):
        for obj in self.active_effects:
            self.canvas.delete(obj)
        self.active_effects = []

    def serial_reader(self):
        while self.running:
            if self.ser.in_waiting:
                try:
                    data = self.ser.readline().decode().strip()
                    print(data)
                    if data in ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']:
                        self.window.after(0, self.start_animation, data)
                except:
                    pass
            sleep(0.01)

    def update_animations(self):
        for system in self.particle_systems[:]:
            if not system.update():
                self.particle_systems.remove(system)
        self.window.after(16, self.update_animations)

    def clean_exit(self):
        self.running = False
        self.thread.join()
        self.ser.close()
        self.window.destroy()


if __name__ == "__main__":
    animator = AdvancedAnimator(port='COM38')
    animator.window.mainloop()
