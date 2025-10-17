# Testing Guide - DaVinci Image Editor

## 🧪 How to Test the Fixes

### Prerequisites
1. Build and run the application
2. Have test images ready (different sizes, formats)

---

## ✅ Test 1: Merge Filter

**Before:** Not working - no UI to select second image

**Steps to Test:**
1. Open an image (e.g., 800x600 pixels)
2. Click "Merge Images" button
3. Select another image of the SAME dimensions (800x600)
4. Verify the images are merged (averaged pixels)

**Expected Result:**
- File dialog opens to select second image
- If dimensions match: images merge successfully
- If dimensions differ: Error message "Images must have the same dimensions to merge!"

---

## ✅ Test 2: Darken/Lighten Filter

**Before:** Broken - always passed 0.0 (no effect)

**Steps to Test:**
1. Open an image
2. Click "Darken / Lighten" button
3. Enter -50 (to darken)
4. Click OK and observe the image
5. Apply again with +50 (to lighten)

**Expected Result:**
- Dialog prompts for percentage (-100 to +100)
- -50 should darken the image significantly
- +50 should brighten the image
- 0 should leave it unchanged

---

## ✅ Test 3: Red Tint Filter

**Before:** Broken - intensity logic was wrong

**Steps to Test:**
1. Open a colorful image
2. Click "Red Tint" button
3. Enter 50 for medium intensity
4. Try again with 100 for maximum red tint

**Expected Result:**
- Dialog prompts for intensity (0-100)
- 50: Image has noticeable red tint
- 100: Image is very red (green/blue completely removed)
- 0: No tint (unchanged)

---

## ✅ Test 4: Crop Filter

**Before:** Broken - cropped (0,0,width,height) = no actual crop

**Steps to Test:**
1. Open an image (e.g., 800x600)
2. Click "Crop" button
3. Enter:
   - X: 100
   - Y: 100
   - Width: 400
   - Height: 300
4. Verify the cropped region

**Expected Result:**
- Four dialog prompts for X, Y, width, height
- Image is cropped to the specified rectangle
- Status bar shows "Applied: Crop"

---

## ✅ Test 5: Resize Filter

**Before:** Only worked once, had height bounds bug

**Steps to Test:**
1. Open an image (e.g., 800x600)
2. Click "Resize" button
3. Enter width: 400, height: 300
4. Verify the image is 400x300
5. **Apply resize AGAIN** with width: 200, height: 150
6. Verify the image is now 200x150

**Expected Result:**
- Two dialog prompts for width and height
- Image resizes correctly the first time
- **CRITICAL:** Image resizes correctly the SECOND time too (this was the bug)
- Can resize multiple times without issues

---

## ✅ Test 6: Infrared Filter

**Before:** Gave blue tint instead of infrared effect

**Steps to Test:**
1. Open a landscape or portrait photo
2. Click "Infrared" button
3. Observe the colors

**Expected Result:**
- Image should have warm, reddish tones (like infrared photography)
- Red channel enhanced (brighter)
- Blue/green channels reduced
- **NOT a blue tint** (that was the bug)

---

## 🎨 UI/UX Tests

### Test 7: Filter Organization
**Verify:**
- ✅ Filters are grouped in categories with emoji icons
- ✅ Sections: Color Adjustments, Effects, Transformations, Frames, Advanced
- ✅ Clean, organized layout

### Test 8: Button Styling
**Verify:**
- ✅ Buttons are blue with white text
- ✅ Buttons turn darker blue on hover
- ✅ Buttons are 35px tall (easy to click)
- ✅ Tooltips appear on hover

### Test 9: Status Feedback
**Verify:**
1. On startup: "Ready - Open an image to begin editing"
2. After opening: "Loaded: filename.jpg (WIDTHxHEIGHT)"
3. After filter: "Applied: [Filter Name]"
4. Clicking filter without image: "Please open an image first!"
5. After save: "Image saved successfully: [path]"

### Test 10: Window Branding
**Verify:**
- ✅ Window title is "DaVinci Image Editor" (not "MainWindow")
- ✅ About dialog shows features and proper description
- ✅ Dock widget has "Filters" title

---

## 💾 Save Functionality Test

### Test 11: Save As
**Before:** Not implemented

**Steps to Test:**
1. Open an image
2. Apply some filters (e.g., Grayscale)
3. File → Save As
4. Choose a filename and format (PNG, JPG, BMP, or TGA)
5. Click Save

**Expected Result:**
- File dialog opens with format filters
- Image saves in selected format
- Status bar: "Image saved successfully: [filename]"
- Can verify the saved file exists

---

## 🔍 Edge Cases to Test

### Test 12: Invalid Inputs
1. **Crop out of bounds:**
   - Try X=700, Y=500, Width=200, Height=200 on a 800x600 image
   - Should work (within bounds)
   
2. **Resize to extreme values:**
   - Try width=10, height=10 (very small)
   - Try width=5000, height=5000 (very large)
   - Should handle gracefully

3. **Merge different dimensions:**
   - Open 800x600 image
   - Try to merge with 1024x768 image
   - Should show error: "Images must have the same dimensions to merge!"

### Test 13: Multiple Filter Applications
1. Open image
2. Apply Grayscale
3. Apply Blur
4. Apply Rotate
5. Apply Resize
6. Apply Crop
7. Save result

**Expected:** All filters work in sequence without crashes

---

## 📊 Quick Checklist

Use this checklist to verify all fixes:

- [ ] ✅ Merge: File dialog opens, works with same-size images
- [ ] ✅ Darken/Lighten: Dialog prompts for -100 to +100, works correctly
- [ ] ✅ Red Tint: Dialog prompts for 0-100, creates proper red tint
- [ ] ✅ Crop: Four dialogs (X,Y,W,H), crops correctly
- [ ] ✅ Resize: Two dialogs (W,H), works MULTIPLE times
- [ ] ✅ Infrared: Creates warm/red tones (NOT blue)
- [ ] ✅ UI: Organized categories with emoji icons
- [ ] ✅ UI: Blue buttons with hover effects
- [ ] ✅ UI: Tooltips on all buttons
- [ ] ✅ UI: Status bar feedback for all actions
- [ ] ✅ Branding: "DaVinci Image Editor" title
- [ ] ✅ Save As: Works and saves files
- [ ] ✅ About: Shows proper features list

---

## 🐛 Known Issues (If Any)

Currently: **None** - All reported issues have been fixed!

---

## 📝 Test Report Template

```
Date: ___________
Tester: ___________

| Test # | Feature | Status | Notes |
|--------|---------|--------|-------|
| 1 | Merge Filter | ⬜ Pass ⬜ Fail | |
| 2 | Darken/Lighten | ⬜ Pass ⬜ Fail | |
| 3 | Red Tint | ⬜ Pass ⬜ Fail | |
| 4 | Crop | ⬜ Pass ⬜ Fail | |
| 5 | Resize (multiple) | ⬜ Pass ⬜ Fail | |
| 6 | Infrared | ⬜ Pass ⬜ Fail | |
| 7 | UI Organization | ⬜ Pass ⬜ Fail | |
| 8 | Button Styling | ⬜ Pass ⬜ Fail | |
| 9 | Status Feedback | ⬜ Pass ⬜ Fail | |
| 10 | Branding | ⬜ Pass ⬜ Fail | |
| 11 | Save As | ⬜ Pass ⬜ Fail | |

Overall Result: ⬜ PASS ⬜ FAIL
```

Happy Testing! 🎉
