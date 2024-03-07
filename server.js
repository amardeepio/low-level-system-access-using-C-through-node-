const sysinfo = require('/home/amardeep/Documents/node_system_access/node_system_access/build/Release/sysinfo_addon.node');
const http = require('http');

const server = http.createServer((req, res) => {
    if (req.url === '/') {
        // Serve the index.html file
        const fs = require('fs');
        const path = require('path');
        const indexPath = path.join(__dirname, 'index.html');
        fs.readFile(indexPath, (err, data) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.write('Internal Server Error');
                res.end();
            } else {
                res.writeHead(200, { 'Content-Type': 'text/html' });
                res.write(data);
                res.end();
            }
        });
    } else if (req.url === '/systeminfo') {
        // Get system information from the addon
        const systemInfo = JSON.parse(sysinfo.getSystemInfo());

        // Send system information as JSON
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(systemInfo));
    } else {
        // Handle 404 Not Found
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.write('404 Not Found');
        res.end();
    }
});

const port = 3000;
server.listen(port, () => {
    console.log(`Server is listening at http://localhost:${port}`);
});
