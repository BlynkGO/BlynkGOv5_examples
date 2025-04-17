import os
import subprocess
from PIL import Image

# ------------------ CONFIG ------------------
TARGET_SIZE = (30, 30)
folder_path = '.'
imgconv_path = r"C:\php\imgconv.py"
declare_list = []
# --------------------------------------------

for filename in os.listdir(folder_path):
    if filename.endswith('-logo.png'):
        # แปลงชื่อไฟล์
        input_path = os.path.join(folder_path, filename)
        base_name = filename.replace('-logo.png', '')
        resized_filename = f"{base_name}_s.png"
        output_path = os.path.join(folder_path, resized_filename)

        # Resize
        try:
            with Image.open(input_path) as img:
                resized_img = img.resize(TARGET_SIZE, Image.Resampling.LANCZOS)
                resized_img.save(output_path)
            print(f"✅ Resized: {filename} → {resized_filename}")
        except Exception as e:
            print(f"❌ Error resizing {filename}: {e}")
            continue

        # เตรียมชื่อ output สำหรับ imgconv
        img_name = f"img_{base_name}_s".replace('-', '_')

        try:
            subprocess.run(
                ["python", imgconv_path, resized_filename, "-o", img_name, "-t"],
                check=True
            )
            print(f"🎉 Converted with imgconv: {resized_filename} → {img_name}.c/.bin")
            declare_list.append(f"IMAGE_DECLARE({img_name});")
        except subprocess.CalledProcessError as e:
            print(f"❌ imgconv failed on {resized_filename}: {e}")
        except FileNotFoundError:
            print(f"❌ ไม่พบ python หรือ imgconv.py")

# 🔽 สรุป IMAGE_DECLARE(...) ทั้งหมด
print("\n📦 Summary of IMAGE_DECLARE():")
for decl in declare_list:
    print(decl)

# 🔽 บันทึกลงไฟล์ image_declare.h (optional)
with open("image_declare.h", "w") as f:
    f.write("// Auto-generated IMAGE_DECLARE list\n\n")
    for decl in declare_list:
        f.write(decl + "\n")
