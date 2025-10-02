# Real-World Use Cases for the Game Engine

This document provides detailed examples of how to use the Game Engine for actual Balatro gameplay scenarios.

## Use Case 1: Finding a Good Money Seed

**Goal**: Find a seed where you can get $200 by ante 4 to buy an expensive legendary joker.

### Step-by-Step

```javascript
// Test multiple seeds
const targetMoney = 200;
const targetAnte = 4;
const goodSeeds = [];

const seedsToTest = [
    "RICH0001", "RICH0002", "RICH0003", "MONEY01", "MONEY02",
    "CASH123", "DOLLAR1", "WEALTH1", "PROFIT1", "FUNDS01"
];

for (const seed of seedsToTest) {
    // Initialize engine with seed
    const engine = new Module.GameEngine(seed);
    
    // Set up state - using Yellow Deck for extra starting money
    const state = new Module.GameState();
    state.deck = "Yellow Deck";  // +$10 starting
    state.stake = "White Stake";
    engine.setState(state);
    
    // Add the money goal
    engine.addGoal(new Module.GameGoal("min_money", targetMoney, targetAnte, true));
    
    // Check if it's feasible
    const feasible = engine.checkGoalFeasibility();
    const feedback = engine.getFeedback();
    
    if (feasible || feedback.includes("FEASIBLE") || feedback.includes("POSSIBLE")) {
        // Also check what vouchers are available
        const voucher1 = engine.predictVoucher(1);
        const voucher2 = engine.predictVoucher(2);
        
        goodSeeds.push({
            seed: seed,
            feasible: feasible,
            voucher1: voucher1,
            voucher2: voucher2,
            feedback: feedback
        });
    }
}

// Report findings
console.log(`Found ${goodSeeds.length} promising seeds:`);
for (const result of goodSeeds) {
    console.log(`\nSeed: ${result.seed}`);
    console.log(`  Early vouchers: ${result.voucher1}, ${result.voucher2}`);
    console.log(`  ${result.feedback}`);
}
```

### Output Example
```
Found 3 promising seeds:

Seed: MONEY01
  Early vouchers: Overstock, Crystal Ball
  min_money_ante_4: FEASIBLE - Target money achievable

Seed: WEALTH1
  Early vouchers: Hone, Tarot Merchant
  min_money_ante_4: POSSIBLE - Target money achievable with good strategy

Seed: PROFIT1
  Early vouchers: Grabber, Telescope
  min_money_ante_4: FEASIBLE - Target money achievable
```

## Use Case 2: Mid-Game Emergency Check

**Scenario**: You're at ante 3 with only $45. The boss blind requires 50k score. Can you buy a $40 joker and still survive ante 4?

```javascript
// Current game state
const currentSeed = "MYRUN01";
const engine = new Module.GameEngine(currentSeed);

// Set current state
const state = new Module.GameState();
state.currentAnte = 3;
state.money = 45;
state.score = 15000;
state.handSize = 8;
state.discards = 3;
state.hands = 4;
state.jokers = ["Joker", "Greedy Joker"];
state.activeVouchers = ["Overstock"];
engine.setState(state);

// Two scenarios: buy or don't buy
console.log("Scenario A: Buy the $40 joker");
state.money = 5;  // After buying
engine.setState(state);
engine.addGoal(new Module.GameGoal("min_money", 30, 4, true));  // Need $30 for next joker
engine.addGoal(new Module.GameGoal("min_score", 100000, 4, true));  // Need to beat ante 4
const feasibleBuy = engine.checkGoalFeasibility();
console.log("Can survive: " + (feasibleBuy ? "YES" : "RISKY"));
console.log(engine.getFeedback());

// Reset and test not buying
engine.clearGoals();
state.money = 45;  // Keep the money
engine.setState(state);
engine.addGoal(new Module.GameGoal("min_score", 100000, 4, true));
console.log("\nScenario B: Keep the money");
const feasibleSave = engine.checkGoalFeasibility();
console.log("Can survive: " + (feasibleSave ? "YES" : "RISKY"));
console.log(engine.getFeedback());

// Get recommendation
const recs = engine.getRecommendedActions(3);
console.log("\nRecommendations:");
for (let i = 0; i < recs.size(); i++) {
    console.log("  - " + recs.get(i));
}
```

## Use Case 3: Challenge Run Planning

**Challenge**: Beat ante 8 on Gold Stake without spending more than $150 total.

```javascript
const seed = "CHALLENGE";
const engine = new Module.GameEngine(seed);

const state = new Module.GameState();
state.deck = "Red Deck";
state.stake = "Gold Stake";
engine.setState(state);

// Set budget constraint
engine.addGoal(new Module.GameGoal("max_money", -150, 8, true));  // Negative = total spent

// Set survival goals
for (let ante = 1; ante <= 8; ante++) {
    const requiredScore = Math.pow(1.5, ante) * 300;  // Rough estimate
    engine.addGoal(new Module.GameGoal("min_score", requiredScore, ante, false));
}

// Check if challenge is possible
const feasible = engine.checkGoalFeasibility();
console.log("Challenge feasible: " + (feasible ? "YES" : "VERY DIFFICULT"));

// Simulate to see critical points
const report = engine.simulateToAnte(8);
console.log("\n" + report);

// Get specific information for planning
console.log("\nKey Information:");
for (let ante = 1; ante <= 8; ante++) {
    const boss = engine.predictBoss(ante);
    const voucher = engine.predictVoucher(ante);
    console.log(`Ante ${ante}: Boss=${boss}, Voucher=${voucher}`);
}
```

## Use Case 4: Synergy Hunting

**Goal**: Find a seed where Blueprint appears early AND you can afford it.

```javascript
function findBlueprintSeed(maxSeeds = 100) {
    for (let i = 1; i <= maxSeeds; i++) {
        const seed = "SEED" + String(i).padStart(4, '0');
        const engine = new Module.GameEngine(seed);
        
        const state = new Module.GameState();
        state.deck = "Red Deck";
        engine.setState(state);
        
        // Check first 30 shop items in ante 1
        const items = engine.predictNextShopItems(30, 1);
        let blueprintFound = false;
        let blueprintPosition = -1;
        
        for (let j = 0; j < items.size(); j++) {
            if (items.get(j) === "Blueprint") {
                blueprintFound = true;
                blueprintPosition = j + 1;
                break;
            }
        }
        
        if (blueprintFound && blueprintPosition <= 15) {
            // Found Blueprint in first 15 items
            // Check if we can afford it (roughly $8-10 per reroll)
            const rerollsNeeded = Math.ceil(blueprintPosition / 2);
            const moneyNeeded = 8 + (rerollsNeeded * 5);  // Base price + rerolls
            
            engine.addGoal(new Module.GameGoal("min_money", moneyNeeded, 1, true));
            const feasible = engine.checkGoalFeasibility();
            
            if (feasible) {
                console.log(`✓ FOUND: ${seed}`);
                console.log(`  Blueprint at position ${blueprintPosition}`);
                console.log(`  Estimated cost: $${moneyNeeded}`);
                console.log(`  First boss: ${engine.predictBoss(1)}`);
                return seed;
            }
        }
    }
    
    console.log("No suitable seed found in range");
    return null;
}

// Run the search
const bestSeed = findBlueprintSeed(50);
```

## Use Case 5: Speedrun Route Planning

**Goal**: Plan the optimal path to beat ante 8 as fast as possible.

```javascript
const seed = "SPEEDRUN";
const engine = new Module.GameEngine(seed);

const state = new Module.GameState();
state.deck = "Abandoned Deck";  // No face cards, faster to play
state.stake = "White Stake";
engine.setState(state);

console.log("=== SPEEDRUN ROUTE PLANNING ===\n");

// For each ante, identify skip opportunities
for (let ante = 1; ante <= 8; ante++) {
    console.log(`Ante ${ante}:`);
    
    // Check boss
    const boss = engine.predictBoss(ante);
    console.log(`  Boss: ${boss}`);
    
    // Check if we can skip shop
    const voucher = engine.predictVoucher(ante);
    const tag = engine.query("tag", ante);
    console.log(`  Voucher: ${voucher} (${voucher.includes('Overstock') ? 'IMPORTANT' : 'SKIP OK'})`);
    console.log(`  Tag: ${tag} (${tag.includes('Skip') ? 'FREE SKIP' : 'no skip'})`);
    
    // Check pack value
    const pack = engine.query("pack", ante);
    console.log(`  Pack: ${pack} (${pack.includes('Buffoon') ? 'TAKE IT' : 'consider skipping'})`);
    
    // Scoring requirement
    const minScore = Math.pow(2, ante - 1) * 300;
    console.log(`  Min Score Needed: ${minScore}`);
    console.log("");
}

console.log("Strategy Notes:");
console.log("- Skip all non-Buffoon packs");
console.log("- Only buy essential jokers");
console.log("- Take Skip tags when available");
console.log("- Aim for mult-stacking jokers early");
```

## Use Case 6: Recovery Planning

**Scenario**: Bad RNG in early game. You're at ante 2 with no good jokers. Can you recover?

```javascript
const seed = "BADSTART";
const engine = new Module.GameEngine(seed);

// Current dire situation
const state = new Module.GameState();
state.currentAnte = 2;
state.money = 15;
state.score = 800;
state.jokers = ["Joker"];  // Just the basic joker
engine.setState(state);

console.log("=== RECOVERY ANALYSIS ===");
console.log("Current situation: Ante 2, $15, 1 basic joker\n");

// Check upcoming opportunities
console.log("Upcoming opportunities:");
for (let ante = 2; ante <= 4; ante++) {
    const items = engine.predictNextShopItems(10, ante);
    console.log(`\nAnte ${ante} shop (first 10 items):`);
    
    let goodJokers = 0;
    for (let i = 0; i < items.size(); i++) {
        const item = items.get(i);
        // Check for good scaling jokers
        if (["Blueprint", "Brainstorm", "Baron", "Fibonacci", "Steel Joker"].includes(item)) {
            console.log(`  Position ${i+1}: ${item} ⭐`);
            goodJokers++;
        }
    }
    
    if (goodJokers === 0) {
        console.log("  No high-priority jokers in first 10 items");
    }
    
    const voucher = engine.predictVoucher(ante);
    console.log(`  Voucher: ${voucher}`);
}

// Set recovery goals
engine.addGoal(new Module.GameGoal("min_money", 50, 3, false));
engine.addGoal(new Module.GameGoal("min_score", 100000, 4, true));

console.log("\nRecovery feasibility:");
const feasible = engine.checkGoalFeasibility();
console.log(feasible ? "✓ Recovery possible" : "⚠ Very difficult");
console.log("\n" + engine.getFeedback());

// Get specific recommendations
console.log("\nRecommendations:");
const recs = engine.getRecommendedActions(2);
for (let i = 0; i < recs.size(); i++) {
    console.log("  " + recs.get(i));
}
```

## Use Case 7: Multi-Goal Optimization

**Goal**: Find the best seed that satisfies multiple constraints.

```javascript
function scoreSeed(seed) {
    const engine = new Module.GameEngine(seed);
    const state = new Module.GameState();
    state.deck = "Blue Deck";
    state.stake = "Red Stake";
    engine.setState(state);
    
    let score = 0;
    
    // Goal 1: $200 by ante 4
    engine.addGoal(new Module.GameGoal("min_money", 200, 4, true));
    if (engine.checkGoalFeasibility()) score += 10;
    
    // Goal 2: Easy first boss
    const boss1 = engine.predictBoss(1);
    if (["Small Blind", "The Hook", "The Club"].includes(boss1)) score += 5;
    
    // Goal 3: Good early voucher
    const voucher1 = engine.predictVoucher(1);
    if (["Overstock", "Hone", "Grabber"].includes(voucher1)) score += 5;
    
    // Goal 4: Blueprint in first 20 items
    const items = engine.predictNextShopItems(20, 1);
    for (let i = 0; i < items.size(); i++) {
        if (items.get(i) === "Blueprint" && i < 15) {
            score += 15;
            break;
        }
    }
    
    return score;
}

// Test multiple seeds
const seeds = [];
for (let i = 1; i <= 50; i++) {
    const seed = "OPT" + String(i).padStart(4, '0');
    const score = scoreSeed(seed);
    seeds.push({ seed, score });
}

// Sort by score
seeds.sort((a, b) => b.score - a.score);

console.log("Top 5 seeds:");
for (let i = 0; i < 5; i++) {
    console.log(`${i+1}. ${seeds[i].seed} (score: ${seeds[i].score})`);
}
```

## Tips for Using These Examples

1. **Adapt to Your Needs**: These examples are templates. Adjust the goals and constraints based on your specific situation.

2. **Iterate**: Run multiple times with different parameters to find the sweet spot.

3. **Combine Techniques**: Use multiple use cases together (e.g., synergy hunting + budget constraints).

4. **Trust the Feedback**: The engine's feedback is based on conservative estimates. "POSSIBLE" usually means it's achievable with good play.

5. **Test Before Committing**: Always simulate a few antes ahead before making big decisions in your actual game.

## Performance Notes

- Testing many seeds (100+) can take a few seconds
- Predicting large numbers of shop items (50+) is slower
- Simulation to ante 8 is faster than simulating each ante individually

## Next Steps

- Explore the [GAME_ENGINE.md](GAME_ENGINE.md) for complete API documentation
- Try the interactive interface in [game_engine_example.html](game_engine_example.html)
- Create your own custom use cases based on your playstyle
