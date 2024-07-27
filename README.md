# Smart Glasses - Team 36
![Product photo](images\product_photo.jpg)
Samsung Innovation Campus AI development Bootcamp (May - Aug 2024) 

This project aims to develop IoT-based smart glasses for the blind using the ESP32 microcontroller. These glasses are equipped with a camera, proximity sensor, speaker and AI technology. The glasses will collect images from the surrounding environment, then AI will detect any objects in the surrounding environment, and provide sound for any objects in the surrounding environment to provide information to the blind who wear glasses.

## Requirements
### Hardware
- ESP32 Camera Module
- Ultrasonic sensor
- Power source

### Application:
- Streamlit
- HTTP
- Flask
- ESP32 IDE (Arduino IDE, ESP-IDF, etc.)
   
### Library (Python)
- YOLOv8 from Ultralytics
- Supervision
- Streamlit
- gTTS (Text-to-Speech)
- Flask
- CV2

### Library (C++ for ESP)
> Make sure you have add this additional boards manager URL to your settings. https://dl.espressif.com/dl/package_esp32_index.json
- WiFi.h
- esp_camera.h
- HTTPClient.h
- stdint.h

## HOW TO USE
1. Clone this repository
   ```bash
   git clone https://github.com/DzuuHilman/Team36FinalProject
   ```
2. Install requirements.txt for python
   ```bash
   pip install -r requirements.txt
   ```
3. Install library requirements for C++
4. Set WiFi credential and HTTP Adress. See [config.h](src\config.h)
   ```C++
   .
   8   // WiFi credential
   9   #define wifi_ssid "Hey hey"          // Change with your WiFi name
   10  #define wifi_pass "aingmaung"        // Change with your WiFi pass
   .
   .
   14  #define http_post_server "http://192.168.161.107:5000/esp32/post_images"
                                        ^^^^^^^^^^^^^^^  change with your IP
5. Compile and upload [src.ino](src/src.ino) to your ESP32 Camera Module
6. Run Flask and Streamlit
   ```bash
   python flask_server.py
   ```
   ```
   streamlit run web.py
   ```
---
## Dataset
This project uses the Ultralytics YOLOv8 library. Ultralytics provides support for various datasets to facilitate computer vision tasks such as detection, instance segmentation, pose estimation, classification, and multi-object tracking. Below is a list of the main Ultralytics datasets, followed by a summary of each computer vision task and the respective datasets.

Bounding box object detection is a computer vision technique that involves detecting and localizing objects in an image by drawing a bounding box around each object.

- Argoverse: A dataset containing 3D tracking and motion forecasting data from urban environments with rich annotations.
- COCO: Common Objects in Context (COCO) is a large-scale object detection, segmentation, and captioning dataset with 80 object categories.
- LVIS: A large-scale object detection, segmentation, and captioning dataset with 1203 object categories.
- COCO8: A smaller subset of the first 4 images from COCO train and COCO val, suitable for quick tests.
- Global Wheat 2020: A dataset containing images of wheat heads for the Global Wheat Challenge 2020.
- Objects365: A high-quality, large-scale dataset for object detection with 365 object categories and over 600K annotated images.
- OpenImagesV7: A comprehensive dataset by Google with 1.7M train images and 42k validation images.
- SKU-110K: A dataset featuring dense object detection in retail environments with over 11K images and 1.7 million bounding boxes.
- VisDrone: A dataset containing object detection and multi-object tracking data from drone-captured imagery with over 10K images and video sequences.
- VOC: The Pascal Visual Object Classes (VOC) dataset for object detection and segmentation with 20 object classes and over 11K images.
- xView: A dataset for object detection in overhead imagery with 60 object categories and over 1 million annotated objects.
- Roboflow 100: A diverse object detection benchmark with 100 datasets spanning seven imagery domains for comprehensive model evaluation.
- Brain-tumor: A dataset for detecting brain tumors includes MRI or CT scan images with details on tumor presence, location, and characteristics.
- African-wildlife: A dataset featuring images of African wildlife, including buffalo, elephant, rhino, and zebras.
- Signature: A dataset featuring images of various documents with annotated signatures, supporting document verification and fraud detection research.
