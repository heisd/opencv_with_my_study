import torch
import os

def export_yolo_onnx():
    # Get the directory where this script is located
    script_dir = os.path.dirname(os.path.abspath(__file__))
    output_path = os.path.join(script_dir, "yolov5s.onnx")
    
    # Directory already exists since the script is in it
    # os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    print(f"Starting export of YOLOv5s to {output_path}...")
    
    try:
        # Load model from torch hub (force_reload=False to use cache if available)
        # Using ultralytics/yolov5 repository
        print("Loading YOLOv5s model from torch.hub...")
        model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True)
        
        # Create dummy input for tracing
        dummy_input = torch.randn(1, 3, 640, 640)
        
        # Export to ONNX
        # opset_version=10 is the most stable for OpenCV
        print("Exporting to ONNX (Opset 10)...")
        torch.onnx.export(model, 
                          dummy_input, 
                          output_path, 
                          opset_version=10,
                          do_constant_folding=True,
                          input_names=['images'],
                          output_names=['output'])
                          
        print(f"Success! Model saved to {output_path}")
        print("You can now run ./opencvyolo.exe")
        
    except Exception as e:
        print(f"Error during export: {e}")
        print("\nPossible solutions:")
        print("1. Ensure you have internet connection (torch.hub needs to download weights)")
        print("2. Install requirements: pip install torch torchvision pandas requests")

if __name__ == "__main__":
    export_yolo_onnx()
