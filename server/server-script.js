const express = require('express');
const fs = require('fs');

const app = express();
const dataFilePath = 'walkthroughs.json';

app.set('view engine', 'ejs');

// Gets today's walkthroughs from the file
function getWalkthroughsForToday() {
    let walkthroughsData = getWalkthroughsFromJson();
    const today = new Date().toISOString().split('T')[0]; // Get today's date in YYYY-MM-DD format
    return walkthroughsData.filter(walkthrough => walkthrough.timestamp && walkthrough.timestamp.startsWith(today));
}

// send /add POST request to add a new walkthrough
app.post('/add', (req, res) => {
    const newWalkthrough = {
        timestamp: new Date()
    };

    try {
        let data = getWalkthroughsFromJson();
        data.push(newWalkthrough);
        fs.writeFileSync(dataFilePath, JSON.stringify(data));
        res.send('New walkthrough added successfully.\n');
    } catch (error) {
        console.error('Error reading data file:', error);
        res.status(500).send('Internal Server Error');
    }
});

// Loads the walkthrough data from the json file
function getWalkthroughsFromJson() {
    try { // Check if there is valid data, if not, return an empty field
        const fileContent = fs.readFileSync(dataFilePath, 'utf8').trim();
        if (fileContent) {
            let walkthroughsData = JSON.parse(fileContent); // Parse JSON into data
            return walkthroughsData;
        } else {
            return [];
        }
    } catch (error) {
        console.error('Error parsing JSON file:', error);
        return []; // Return an empty array to handle the error gracefully
    }
}

// Normal GET request returns the webpage
app.get('/', (req, res) => {
const walkthroughsToday = getWalkthroughsForToday();
    const dataToSend = {
        walkthroughsToday: walkthroughsToday.length
    };
    res.render('index', { data: dataToSend });
});

// Keeps listening for changes in the walkthroughs file
fs.watch('./walkthroughs.json', (event, filename) => {
    console.log(`${filename} file changed.`);
});

// Starts the server on port 10000
const PORT = process.env.PORT || 10000;
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});