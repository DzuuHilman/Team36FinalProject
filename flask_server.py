from flask import Flask, request, jsonify, send_file
import os
import time
from gtts import gTTS
import supervision as sv
from ultralytics import YOLO
import cv2
from scipy.ndimage import rotate
import numpy as np

app = Flask(__name__)

frames_folder = 'frames'
os.makedirs(frames_folder, exist_ok=True)

tts_folders = 'tts'
os.makedirs(tts_folders, exist_ok=True)

model = YOLO('yolov8s.pt')
bboxannotator = sv.BoxAnnotator()
labels = []

# Function to get TTS audio
def get_tts_voice():
    global labels
    if not labels:
        return jsonify({'error': 'No object detected'}), 200

    object_name = labels[0]

    voiceCall = "Careful! There is a " + object_name + "in front of you!"
    tts = gTTS(text=voiceCall, lang='en')

    filename = 'tts.mp3'  # You can generate a unique name if needed
    filepath = os.path.join(tts_folders, filename)
    
    tts.save(filepath)

@app.route('/')
def landing_page():
    return "This is landing page"


@app.route('/esp32/post_images', methods=['POST'])
def upload_file():
    # Check if the content type is image/jpeg
    if request.content_type != 'image/jpeg':
        return jsonify({'error': 'Invalid content type. Expected image/jpeg'}), 400

    # Get the image data from the request
    image_data = request.get_data()

    if not image_data:
        return jsonify({'error': 'No image data received'}), 400

    # Generate a filename and save the image
    filename = 'frame.jpg'  # You can generate a unique name if needed
    filepath = os.path.join(frames_folder, filename)
    with open(filepath, 'wb') as f:
        f.write(image_data)
    image = cv2.imread(filepath)

    # rotate image 180 degree 
    image = rotate(image, 180)

    # # Pre process image
    # # Resize the image using interpolation
    # scale_percent = 200  # Percent of original size
    # width = int(image.shape[1] * scale_percent / 100)
    # height = int(image.shape[0] * scale_percent / 100)
    # dim = (width, height)
    # resized_image = cv2.resize(image, dim, interpolation=cv2.INTER_LINEAR)

    # # Apply CLAHE (Contrast Limited Adaptive Histogram Equalization)
    # clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    # r, g, b = cv2.split(resized_image)
    # r_clahe = clahe.apply(r)
    # g_clahe = clahe.apply(g)
    # b_clahe = clahe.apply(b)
    # equalized = cv2.merge((r_clahe, g_clahe, b_clahe))

    # # Apply Gamma Correction
    # gamma = 1.2
    # gamma_corrected = np.array(255*(equalized / 255) ** gamma, dtype='uint8')

    # # Apply Brightness and Contrast adjustment with reduced intensity
    # alpha = 1.1  # Reduced contrast control (1.0-3.0)
    # beta = 10    # Reduced brightness control (0-100)
    # adjusted = cv2.convertScaleAbs(gamma_corrected, alpha=alpha, beta=beta)

    # # Apply Edge Enhancement with reduced kernel intensity
    # kernel_edge = np.array([[-0.5, -0.5, -0.5], 
    #                         [-0.5,  5,   -0.5], 
    #                         [-0.5, -0.5, -0.5]])
    # enhanced = cv2.filter2D(adjusted, -1, kernel_edge)

    # # Apply Gaussian Blurring to reduce noise
    # blurred = cv2.GaussianBlur(enhanced, (3, 3), 0)

    # # Final sharpening (if needed)
    # kernel_sharpen = np.array([[0, -0.3, 0], 
    #                         [-0.3, 2, -0.3], 
    #                         [0, -0.3, 0]])
    # sharpened = cv2.filter2D(blurred, -1, kernel_sharpen)

    # # Resize back to original size
    # final_image = cv2.resize(sharpened, (image.shape[1], image.shape[0]), interpolation=cv2.INTER_LINEAR)
    # image = final_image

    # Perform object detection
    result = model(image)[0]
    detections = sv.Detections.from_ultralytics(result)
    detections = detections[detections.confidence > 0.5]
    global labels
    labels = [
        f"{result.names[class_id]}"
        for class_id, confidence in zip(detections.class_id, detections.confidence)
    ]
    boxannotator= sv.BoxAnnotator()
    annotated_image = boxannotator.annotate(scene=image, detections=detections)
    
    # Save the annotated image
    filename = 'annotated_frame.jpg'  # You can generate a unique name if needed
    filepath = os.path.join(frames_folder, filename)
    cv2.imwrite(filepath, annotated_image)    

    print(labels)
    if len(labels) == 0:
        return jsonify({'message': 'No objects detected'}), 200
    else:
        get_tts_voice()
        return jsonify({'labels': labels, 'Message': "Sucsess creating TTS audio! "}), 200 

    # return jsonify({'message': 'File uploaded successfully'}), 200


@app.route('/esp32/get_images', methods=['GET'])
def get_files():
    files = os.listdir(frames_folder)
    global labels
    print(labels)
    return jsonify({'files': files, 'labels': labels}), 200

@app.route('/esp32/post_and_get_tts_voice', methods=['GET'])
def post_and_get_tts_voice():
    global labels
    if not labels:
        return jsonify({'error': 'No object detected'}), 200

    object_name = labels[0]

    voiceCall = "Careful! There is a " + object_name + "in front of you!"
    tts = gTTS(text=voiceCall, lang='en')

    filename = 'tts.mp3'  # You can generate a unique name if needed
    filepath = os.path.join(tts_folders, filename)
    
    tts.save(filepath)
    return send_file('tts/tts.mp3', mimetype='audio/mpeg'), 200

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)