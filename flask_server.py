from flask import Flask, request, jsonify, send_file
from pydub import AudioSegment
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
labels = []

def converter_mp3_to_raw(mp3_filepath, raw_file_destination):
    sound = AudioSegment.from_mp3(mp3_filepath)
    data = sound._data
    with open(raw_file_destination, 'wb') as f:
        f.write(data)
    return data
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
        return jsonify({'message': 'Objects detected', 'labels': labels}), 200 

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
    # if not labels:
    #     return jsonify({'error': 'No object detected'}), 200
    labels.append('person')
    object_name = labels[0]

    voiceCall = "Careful! There is a " + object_name + "in front of you!"
    tts = gTTS(text=voiceCall, lang='en')

    # Generate file name
    filename            ='tts.mp3'
    filepath            = os.path.join(tts_folders, filename)
    raw_filename        ='tts.raw'
    raw_filepath        = os.path.join(tts_folders, raw_filename)
    c_filename          ='tts.c'
    c_filepath          = os.path.join(tts_folders, c_filename)

    tts.save(filepath)

    # convert mp3 to raw
    data = converter_mp3_to_raw(filepath, raw_filepath)

    # Check if convertion is sucsess
    if not os.path.exists(raw_filepath):
        return jsonify({'error': 'Failed to convert .mp3 to RAW'}), 500
    
    # raw to c file conversion and send c file
    with open(c_filepath, 'w') as f:
        f.write('const uint8_t tts_data[] = {')

        # Iterate through the data, converting each byte to hexadecimal
        for i in range(len(data)):
            # Write each byte in hexadecimal, no spaces or newlines
            f.write(f'{data[i]:02X},')

        f.write('};\n')
    
    # Check if the convertion is sucsess
    if not os.path.exists(c_filepath):
        return jsonify({'error': 'Failed to convert RAW to c file'}), 500
    
    # Send the c file
    return send_file(c_filepath, mimetype='text/plain'), 200
    
    # Send the raw data as a file
    # return send_file(filepath, mimetype='audio/mpeg'), 200
    return jsonify({"Data": c_filepath}), 200

if __name__ == '__main__':
    app.run(debug=False, host='0.0.0.0', port=5000)