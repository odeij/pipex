# 🚀 GitHub Setup Guide

## Quick Upload Steps

### 1. Initialize Git (if not already done)
```bash
git init
```

### 2. Add All Files
```bash
git add .
```

### 3. Commit Changes
```bash
git commit -m "feat: Complete pipex implementation with bonus features

- Implemented mandatory pipe functionality
- Added bonus features (heredoc, multiple pipes)
- Created comprehensive documentation
- Added test suite and architecture diagrams"
```

### 4. Create GitHub Repository
1. Go to [GitHub](https://github.com)
2. Click "New repository"
3. Name it `pipex`
4. **Don't** initialize with README (we already have one)
5. Click "Create repository"

### 5. Connect and Push
```bash
# Add remote (replace YOUR_USERNAME with your GitHub username)
git remote add origin https://github.com/YOUR_USERNAME/pipex.git

# Push to GitHub
git branch -M main
git push -u origin main
```

## Repository Settings to Enable

### 1. Add Topics/Tags
Go to repository settings → Topics, add:
- `42`
- `42-school`
- `pipex`
- `c`
- `system-programming`
- `unix`
- `pipes`
- `process-management`

### 2. Add Description
```
Recreate shell pipe functionality in C - 42 School project demonstrating process management, IPC, and file descriptor manipulation
```

### 3. Enable GitHub Pages (Optional)
If you want to showcase the documentation:
- Settings → Pages
- Source: `main` branch
- Folder: `/ (root)`

### 4. Add Repository Badges
You can add these to your README (already included):
- 42 School badge
- C language badge
- Platform badges

## Pre-Push Checklist

- [ ] All code compiles without errors (`make` and `make bonus`)
- [ ] README.md is complete and professional
- [ ] CODE_EXPLANATION.md is included
- [ ] ARCHITECTURE.md is included
- [ ] .gitignore is configured
- [ ] Test script works (`./test.sh`)
- [ ] No sensitive information in code
- [ ] All files follow 42 norminette standards
- [ ] Author information is updated in README.md

## Making Your Repo Stand Out

### 1. Update Author Section
Edit README.md and replace:
- `@yourusername` with your GitHub username
- `@your42login` with your 42 intra login

### 2. Add Screenshots (Optional)
Create a `screenshots/` directory and add:
- Terminal output showing usage
- Test results
- Architecture diagrams

### 3. Add More Examples
Consider adding:
- Complex use cases
- Performance comparisons
- Edge case handling

### 4. Pin the Repository
On your GitHub profile, pin this repository to showcase it.

## Post-Upload

### 1. Share Your Work
- Share on LinkedIn
- Post on 42 forums
- Add to your portfolio

### 2. Keep It Updated
- Fix any bugs you find
- Add improvements
- Respond to issues/PRs

### 3. Star Your Own Repo
It helps with visibility! ⭐

---

**Good luck with your GitHub upload! 🎉**

