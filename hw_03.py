import numpy as np
import cv2

import os.path

hist_size = 256
hist_w = 512
hist_h = 400
hist_thickness = 2
bin_w = int(hist_w / hist_size)

color = ('b', 'g', 'r')
color_value = {"b": (255, 0, 0), "g": (0, 255, 0), "r": (0, 0, 255)}

dst_path = "./src_images/"
dst_name = "road_3.jpg"  # 指定需要判定的圖片
dst = os.path.abspath(os.path.join(dst_path, dst_name))

src_path = "./src_images/"
src_name = ["beach_1.jpg", "beach_2.jpg", "beach_3.jpg",
            "mountain_1.jpg", "mountain_2.jpg", "mountain_3.jpg",
            "restaurant_1.jpg", "restaurant_2.jpg", "restaurant_3.jpg",
            "road_1.jpg", "road_2.jpg", "road_3.jpg"]
srcs = map(lambda src: os.path.abspath(os.path.join(src_path, src)), src_name)
scenes = ["beach", "mountain", "restaurant", "road"]


def calculate_histogram(img):
    hist = []
    for i, col in enumerate(color):
        hist.append(cv2.calcHist([img], [i], None, [hist_size], [0, hist_size]))
        cv2.normalize(hist[i], hist[i], 0, hist_w, cv2.NORM_MINMAX, -1)
    return hist


def draw_histogram(hist):
    hist_img = np.zeros((hist_h, hist_w, 3), np.uint8)
    for c, col in enumerate(color):
        hist_img = cv2.line(hist_img,
                            (bin_w * (0), hist_h - hist[c][0]),
                            (bin_w * (0), hist_h - hist[c][0]),
                            color_value[col], hist_thickness)
        for i, v in enumerate(hist[c][1:]):
            hist_img = cv2.line(hist_img,
                                (bin_w * (i-1), hist_h - hist[c][i-1]),
                                (bin_w * (i), hist_h - hist[c][i]),
                                color_value[col], hist_thickness)
    return hist_img


def compare_histogram(src_hist, dst_hist):
    src_sample = []
    dst_sample = []
    correlation = 0
    for i in range(hist_size):
        src_sample.append(0.30 * (src_hist[2])[i] +
                          0.59 * (src_hist[1])[i] +
                          0.11 * (src_hist[0])[i])
        dst_sample.append(0.30 * (dst_hist[2])[i] +
                          0.59 * (dst_hist[1])[i] +
                          0.11 * (dst_hist[0])[i])
        correlation = correlation + abs(src_sample[i] - dst_sample[i]) / ((src_sample[i] + dst_sample[i]) / 2.0)
    correlation = correlation / 256
    return correlation


def method_draw():
    for src in srcs:
        img = cv2.imread(src)
        hist = calculate_histogram(img)
        hist_img = draw_histogram(hist)
        cv2.imwrite(src + ".hist.png", hist_img)


def method_compare():
    hists = list(map(lambda src: calculate_histogram(cv2.imread(src)), srcs))
    dst_hist = calculate_histogram(cv2.imread(dst))
    scenes_correlation = []
    for i, scene in enumerate(scenes):
        scene_correlation = 0
        for j in range(3):
            index = i * 3 + j
            correlation = compare_histogram(hists[index], dst_hist)
            print("{:20} vs. {:20}: {}".format(src_name[index], dst_name, correlation))
            scene_correlation += correlation
        scenes_correlation.append(scene_correlation)
    return scenes[scenes_correlation.index(min(scenes_correlation))]


def main():
    print("IT MIGHT BE:", method_compare())


if __name__ == '__main__':
    main()
