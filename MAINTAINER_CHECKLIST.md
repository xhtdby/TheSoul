# Maintainer Checklist - Game Engine Integration

This checklist helps you verify and deploy the new Game Engine feature.

## Pre-Deployment Checklist

### 1. Code Review
- [ ] Review `include/game_engine.hpp` for correctness
- [ ] Review updates to `include/immolate.hpp` bindings
- [ ] Check that no existing files were broken
- [ ] Verify coding style matches project conventions

### 2. Compilation
- [ ] Install/update Emscripten if needed
- [ ] Run build command:
  ```bash
  em++ -O3 --closure 1 -lembind -o immolate.js include/immolate.cpp -s EXPORT_NAME="'Immolate'"
  ```
- [ ] Verify no compilation errors
- [ ] Check that `immolate.js` and `immolate.wasm` are generated
- [ ] Verify file sizes are reasonable (should increase ~10-20%)

### 3. Testing
- [ ] Open `game_engine_example.html` in browser
- [ ] Verify "Engine Ready" message appears
- [ ] Test initializing with a seed
- [ ] Add a goal and check feasibility
- [ ] Run a simulation
- [ ] Query information (boss, voucher, etc.)
- [ ] Open browser console and paste contents of `integration_test.js`
- [ ] Verify all 10 tests pass

### 4. Documentation Review
- [ ] Read through GAME_ENGINE.md
- [ ] Try examples from QUICKSTART.md
- [ ] Review example_use_case.md scenarios
- [ ] Check README.md updates are accurate

### 5. Backward Compatibility
- [ ] Open original `index.html`
- [ ] Test existing seed analysis features
- [ ] Verify no regression in original functionality
- [ ] Check that old links/bookmarks still work

## Deployment Checklist

### 1. Build Artifacts
- [ ] Commit new `immolate.js` and `immolate.wasm`
- [ ] Update `.gitignore` if needed
- [ ] Tag release (e.g., v2.0.0-game-engine)

### 2. Documentation
- [ ] Add Game Engine section to main page/README
- [ ] Link to `game_engine_example.html` from main site
- [ ] Update changelog
- [ ] Add release notes

### 3. Web Hosting
- [ ] Deploy `game_engine_example.html` to site
- [ ] Update navigation to include new page
- [ ] Test on mobile devices
- [ ] Test on different browsers (Chrome, Firefox, Safari)

### 4. Communication
- [ ] Announce feature in project updates
- [ ] Create tutorial video/GIF (optional)
- [ ] Post examples in community forums
- [ ] Update documentation links

## Post-Deployment Monitoring

### 1. User Feedback
- [ ] Monitor GitHub issues for bug reports
- [ ] Collect feature requests
- [ ] Track usage via analytics (if available)

### 2. Performance
- [ ] Monitor page load times
- [ ] Check memory usage in long sessions
- [ ] Optimize if needed

### 3. Bugs & Fixes
- [ ] Fix any reported issues
- [ ] Add tests for edge cases
- [ ] Update documentation as needed

## Quick Validation Commands

```bash
# 1. Check file structure
ls -lh include/game_engine.hpp
ls -lh game_engine_example.html
ls -lh GAME_ENGINE.md

# 2. Verify no syntax errors (if you have clang)
clang++ -std=c++11 -fsyntax-only include/game_engine.hpp

# 3. Count lines of new code
wc -l include/game_engine.hpp GAME_ENGINE.md game_engine_example.html

# 4. Check git status
git status
git log --oneline -5
```

## Common Issues & Solutions

### Issue: Compilation fails with "undefined reference"
**Solution**: Check include order in `immolate.hpp`. Make sure `items.hpp` comes before `instance.hpp` and `game_engine.hpp`.

### Issue: "GameEngine is not defined" in browser
**Solution**: 
1. Check that `immolate.js` was recompiled with new bindings
2. Verify WebAssembly loaded successfully (check browser console)
3. Wait for `onRuntimeInitialized` callback

### Issue: Goals don't work as expected
**Solution**:
1. Check that state was set via `setState()`
2. Verify goal types are correct (min_money, min_score, etc.)
3. Ensure ante numbers are valid (1-8)

### Issue: Simulation returns empty results
**Solution**:
1. Verify engine was initialized with valid seed
2. Check that `simulateToAnte()` ante is >= current ante
3. Ensure state was set properly

### Issue: Existing analyzer broken
**Solution**:
1. This shouldn't happen - Game Engine is isolated
2. Check that original Instance bindings are still present
3. Verify no files were accidentally modified
4. Rebuild from clean state

## Rollback Plan

If major issues arise:

1. **Immediate**: Revert to previous `immolate.js`/`immolate.wasm`
2. **Keep docs**: Documentation can stay (it's informative)
3. **Fix issues**: Address problems in new branch
4. **Re-deploy**: Once fixed, deploy again

## Testing Matrix

| Browser | OS | Tested | Works | Notes |
|---------|----|----|-------|-------|
| Chrome | Windows | [ ] | [ ] | |
| Chrome | macOS | [ ] | [ ] | |
| Chrome | Linux | [ ] | [ ] | |
| Firefox | Windows | [ ] | [ ] | |
| Firefox | macOS | [ ] | [ ] | |
| Safari | macOS | [ ] | [ ] | |
| Safari | iOS | [ ] | [ ] | |
| Chrome | Android | [ ] | [ ] | |

## Performance Benchmarks

Test these scenarios and record times:

| Scenario | Target | Actual | Pass/Fail |
|----------|--------|--------|-----------|
| Initialize engine | < 5ms | | [ ] |
| Add 10 goals | < 10ms | | [ ] |
| Check feasibility | < 50ms | | [ ] |
| Simulate 8 antes | < 100ms | | [ ] |
| Query 100 items | < 50ms | | [ ] |
| Load page | < 3s | | [ ] |

## File Size Check

| File | Before | After | Change |
|------|--------|-------|--------|
| immolate.js | | | |
| immolate.wasm | | | |
| Total | | | |

Acceptable increase: 10-30%

## Final Sign-Off

Before going live:

- [ ] All tests pass
- [ ] No compilation errors
- [ ] No console errors in browser
- [ ] Documentation is complete
- [ ] Examples work correctly
- [ ] Backward compatibility verified
- [ ] Performance is acceptable
- [ ] At least 2 browsers tested
- [ ] Mobile tested (if applicable)
- [ ] Team reviewed and approved

**Signed off by:** _______________
**Date:** _______________
**Version:** _______________

## Support Resources

- Technical Documentation: `GAME_ENGINE.md`
- User Guide: `QUICKSTART.md`
- Examples: `example_use_case.md`
- Implementation Details: `IMPLEMENTATION_SUMMARY.md`
- Test Suite: `integration_test.js`
- CLI Tool: `game_engine_cli.py`

## Next Steps After Deployment

1. Monitor user feedback for first week
2. Fix any critical bugs immediately
3. Collect feature requests for v2.1
4. Plan integration with main UI (optional)
5. Consider mobile app (optional)
6. Explore advanced features (ML, optimization, etc.)

---

**Notes:**
- Keep this checklist updated as you complete items
- Document any issues encountered and solutions
- Share knowledge with team members
- Celebrate successful deployment! 🎉
