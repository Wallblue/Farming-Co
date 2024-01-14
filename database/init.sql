CREATE TABLE IF NOT EXISTS cooperative(
           coopId INTEGER PRIMARY KEY AUTOINCREMENT,
           coopName VARCHAR(25),
           startTime DATETIME,
           maxMembers INTEGER,
           level INTEGER,
           money INTEGER
);

CREATE TABLE IF NOT EXISTS player(
      playerId INTEGER PRIMARY KEY AUTOINCREMENT,
      pseudo VARCHAR(25),
      startDate DATETIME,
      farmName VARCHAR(25),
      timeInGame INTEGER,
      maxEnergy INTEGER,
      coopId INTEGER,
      FOREIGN KEY (coopId) REFERENCES cooperative(coopId)
);

CREATE TABLE IF NOT EXISTS object(
      objectId INTEGER PRIMARY KEY AUTOINCREMENT,
      x INTEGER,
      y INTEGER,
      w INTEGER,
      h INTEGER,
      name VARCHAR(25),
      poseDate INT,
      growDate INT,
      sprite VARCHAR(128),
      state INTEGER,
      boosted BOOLEAN,
      playerId INTEGER,
      FOREIGN KEY (playerId) REFERENCES player(playerId)
);

CREATE TABLE IF NOT EXISTS place(
     placeId INTEGER PRIMARY KEY AUTOINCREMENT,
     name VARCHAR(50),
     type INTEGER
);

CREATE TABLE IF NOT EXISTS npc(
   npcId INTEGER PRIMARY KEY AUTOINCREMENT,
   firstName VARCHAR(25),
   lastName VARCHAR(25),
   skin VARCHAR(128),
   tradable BOOLEAN,
   placeId INTEGER,
   FOREIGN KEY (placeId) REFERENCES place(placeId)
);

CREATE TABLE IF NOT EXISTS item(
    itemId INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(25),
    type VARCHAR(25),
    description TEXT,
    energyBonus INTEGER,
    ability INTEGER,
    quantity INTEGER,
    sprite VARCHAR(128),
    growTime INTEGER,
    ownerId INTEGER,
    npcId INTEGER,
    FOREIGN KEY (ownerId) REFERENCES player(playerId),
    FOREIGN KEY (npcId) REFERENCES npc(npcId)
);

CREATE TABLE IF NOT EXISTS dialog(
      dialogId INTEGER PRIMARY KEY AUTOINCREMENT,
      content TEXT,
      npcId INTEGER,
      FOREIGN KEY (npcId) REFERENCES npc(npcId)
);
