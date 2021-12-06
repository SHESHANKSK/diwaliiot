from flask import Flask, render_template, request, jsonify
import json

app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/form', methods=['POST'])
def hex_to_rgb():
    hex_value = request.form.get('input_color').lstrip('#')
    rgb_tuple = tuple(int(hex_value[i:i + 2], 16) for i in (0, 2, 4))
    red_value = rgb_tuple[0]
    green_value = rgb_tuple[1]
    blue_value = rgb_tuple[2]
    result = {
        "rgb_value": {
            "red_value": int(red_value),
            "green_value": int(green_value),
            "blue_value": int(blue_value)
        }
    }

    with open("output.json", "w") as outfile:
        json.dump(result, outfile)
    return jsonify(result)


@app.route('/api')
def api():
    with open("output.json") as json_file:
        content = json.load(json_file)
    return jsonify(content)


if __name__ == "__main__":
    app.run()
