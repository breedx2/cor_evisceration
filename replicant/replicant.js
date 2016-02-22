"use strict";
const blessed = require('blessed');
const lodash = require('lodash');
const dgram = require('dgram');

const PORT = 2084;

const server = dgram.createSocket('udp4');
server.on('message', handleMessage);
server.bind(PORT);
console.log(`Server listening on port ${PORT}`);

var state = null;

var screen = blessed.screen({
    smartCSR: true
});
screen.key(['escape', 'q', 'C-c'], function(ch, key) {
  return process.exit(0);
});

setInterval(drawState, 25);

function handleMessage(msg, rinfo) {
    msg = msg.toString();
    let lines = msg.split('\n');
    if (lines[0] == 'GAME_STATE') {
        state = decodeState(lines.slice(1));
    }
}

function drawState() {
    if (!state) return;
    let borderBox = blessed.box({
        left: 'center', top: 'center',
        width: 136, height: 52,
        border: { type: 'line' },
        style: {
            border: { fg: 'red' }
        }
    });
    screen.append(borderBox);
    screen.append( blessed.text({
        content:'M', bold: true, left: borderBox.left + state.player[0] + 1, top: borderBox.top + parseInt(state.player[1]/4) + 1
    }))
    drawList(borderBox, state.electrodes, '*', 'magenta');
    drawList(borderBox, state.grunts, 'g', 'red');
    drawList(borderBox, state.hulks, 'H', 'green');
    drawList(borderBox, state.humans, '♁', 'cyan');
    drawList(borderBox, state.spheroids, 'O', 'red');
    drawList(borderBox, state.enforcers, '^', 'white');
    drawList(borderBox, state.sparks, 'x', 'white');
    drawList(borderBox, state.brains, 'B', 'blue');
    screen.render();
}

function drawList(borderBox, list, char, color){
    list.forEach(e => {
        screen.append( blessed.text({
            content: char, fg: color, bold: true, left: borderBox.left + e[0] + 1, top: borderBox.top + parseInt(e[1]/4) + 1
        }))
    });
}

function decodeState(lines) {
    let result = {};
    lines.filter(line => line.trim().length > 0).forEach(line => {
        let parts = line.split(':');
        let key = parts[0];
        switch (key) {
            case 'lives':
            case 'score':
            case 'wave':
                result[key] = parseInt(parts[1]);
                break;
            case 'player':
                result[key] = parsePoint(parts[1]);
                break;
            default:
                result[key] = parsePointList(parts[1]);
        }
    });
    return result;
}

function parsePointList(str) {
    if (!str) return [];
    return str.split(/;/).map(parsePoint);
}

// Given a string (x,y) return [x,y] parsed
function parsePoint(str) {
    return str.replace(/[)(]/, '').split(/,/).map(x => parseInt(x));
}
