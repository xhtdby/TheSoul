# Quick Start Guide - Balatro Game Engine

Welcome! This guide will help you get started with the new Game Engine features in TheSoul.

## What is the Game Engine?

The Game Engine helps you **plan your Balatro runs** by:
1. Setting goals (like "have $150 by ante 4")
2. Checking if those goals are achievable with your seed
3. Getting recommendations on what to do
4. Analyzing your run at any point (not just the start)

## 5-Minute Tutorial

### Step 1: Open the Game Engine

Open `game_engine_example.html` in your web browser.

### Step 2: Initialize with a Seed

1. Enter a seed (e.g., "TEST123")
2. Select your deck and stake
3. Click "Initialize Engine"
4. Wait for "✓ Engine Ready"

### Step 3: Set a Goal

Let's say you want to have at least $100 by ante 3:

1. Select "Minimum Money" from Goal Type
2. Enter "100" in Target Value
3. Enter "3" in By Ante
4. Check "Mandatory" if this is a hard requirement
5. Click "Add Goal"

### Step 4: Check Feasibility

Click "Check If Goals Are Achievable"

The engine will tell you:
- ✓ FEASIBLE - This is achievable
- CHALLENGING - It's possible but difficult
- ✗ UNLIKELY - Very hard or impossible

### Step 5: Simulate Your Run

1. Enter "5" in "Simulate to Ante"
2. Click "Run Simulation"
3. See what bosses, vouchers, and tags you'll get
4. Check if your goals are met at each ante

### Step 6: Get Recommendations

1. Enter your current ante (e.g., "2")
2. Click "Get Recommendations"
3. See strategic advice based on your goals

## Common Use Cases

### Use Case 1: Planning a Fresh Run

**Scenario**: You want to find a good seed for a money-focused run.

**Steps**:
1. Try different seeds (SEED0001, SEED0002, etc.)
2. Set goal: "min_money:200:5:mandatory"
3. Check feasibility for each seed
4. Pick a seed that shows "FEASIBLE" or "POSSIBLE"

### Use Case 2: Mid-Game Decision Making

**Scenario**: You're at ante 3 with $80. Can you afford an expensive joker and still hit $150 by ante 5?

**Steps**:
1. Initialize with your current seed
2. Set current state: Ante 3, Money $80
3. Add goal: "min_money:150:5:mandatory"
4. Check feasibility
5. If feasible, buy the joker. If not, save money.

### Use Case 3: Challenge Runs

**Scenario**: You want to beat ante 8 without spending more than $100 total.

**Steps**:
1. Initialize with seed
2. Add goal: "max_money:100:8:mandatory"
3. Simulate to ante 8
4. Review what items you can afford
5. Plan your purchases carefully

### Use Case 4: Synergy Hunting

**Scenario**: You want Blueprint and need $50 for it by ante 2.

**Steps**:
1. Initialize with seed
2. Query shop items for ante 1 and 2
3. Check if Blueprint appears
4. Add goal: "min_money:50:2"
5. Get recommendations on how to save money

## Goal Types Explained

### min_money
**What**: Minimum amount of money you need to have
**Example**: "I need at least $150 by ante 4"
**Use For**: Ensuring you can buy expensive jokers or vouchers

### min_score
**What**: Minimum score needed to beat a blind
**Example**: "I need 500K score by ante 5"
**Use For**: Checking if you can survive to later antes

### max_money
**What**: Maximum money you can spend (constraint)
**Example**: "Stay under $100 total spending by ante 5"
**Use For**: Challenge runs, budget constraints

### max_score
**What**: Maximum score (anti-scoring constraints)
**Example**: "Don't exceed 1M score before ante 8"
**Use For**: Special challenge modes

## Tips & Tricks

### Tip 1: Multiple Goals
You can add multiple goals! For example:
- min_money:100:3 (have $100 by ante 3)
- min_money:200:5 (have $200 by ante 5)
- min_score:1000000:8 (reach 1M by ante 8)

### Tip 2: Mandatory vs Optional
- **Mandatory**: Engine will warn if impossible
- **Optional**: Engine notes it but doesn't fail

### Tip 3: Mid-Game Analysis
You don't have to start from ante 1! Feed in your current game state:
- Current ante
- Current money
- Current score
- Your jokers
- Active vouchers

### Tip 4: Query Before Deciding
Before making big decisions, query:
- "What boss am I facing next ante?"
- "What voucher is available?"
- "What packs will appear?"

### Tip 5: Iterate on Seeds
Try multiple seeds with the same goals to find the best one for your strategy.

## Advanced Features

### State Management
```javascript
let state = new Module.GameState();
state.currentAnte = 3;
state.money = 150;
state.score = 400000;
state.jokers = ["Blueprint", "Brainstorm"];
state.activeVouchers = ["Overstock"];
engine.setState(state);
```

### Custom Queries
```javascript
// What boss in ante 5?
let boss = engine.query("boss", 5);

// What voucher in ante 3?
let voucher = engine.query("voucher", 3);

// What tag in ante 2?
let tag = engine.query("tag", 2);
```

### Shop Prediction
```javascript
// Get next 10 shop items for ante 2
let items = engine.predictNextShopItems(10, 2);
for (let i = 0; i < items.size(); i++) {
    console.log(items.get(i));
}
```

## Troubleshooting

**Q: "Engine not initialized" error**
A: Make sure to click "Initialize Engine" after entering a seed

**Q: Goals show as "UNLIKELY" but I know they're possible**
A: The feasibility check is conservative. It may underestimate what's achievable with perfect play.

**Q: Simulation doesn't show exact shop items**
A: The main simulation focuses on major events. Use "Query Specific Information" for detailed shop predictions.

**Q: Can I save my goals?**
A: Currently no, but you can bookmark URLs (planned feature).

**Q: Does this work with the original TheSoul interface?**
A: The Game Engine is a separate interface. Integration planned for future release.

## What's Next?

After mastering the basics:
1. Read [GAME_ENGINE.md](GAME_ENGINE.md) for full API documentation
2. Try the CLI version: `python3 game_engine_cli.py --help`
3. Experiment with complex multi-goal scenarios
4. Share your best seeds and strategies!

## Support

For questions, issues, or feature requests:
- Open an issue on GitHub
- Check existing documentation
- Try the examples in `game_engine_example.html`

Happy planning! 🃏
