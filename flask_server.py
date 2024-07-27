from flask import Flask, request, jsonify, send_file
import os
import time
from gtts import gTTS
import supervision as sv
from ultralytics import YOLO

app = Flask(__name__)

frames_folder = 'frames'
os.makedirs(frames_folder, exist_ok=True)

tts_folders = 'tts'
os.makedirs(tts_folders, exist_ok=True)

model = YOLO('yolov8s.pt')
labels = ["person"]

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
        
    # Perform object detection
    result = model(filepath)[0]
    detections = sv.Detections.from_ultralytics(result)
    detections = detections[detections.confidence > 0.5]
    global labels
    labels = [
        f"{result.names[class_id]}"
        for class_id, confidence in zip(detections.class_id, detections.confidence)
    ]

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