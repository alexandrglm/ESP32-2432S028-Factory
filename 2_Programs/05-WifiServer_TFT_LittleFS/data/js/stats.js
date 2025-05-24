
let cpuChart, memoryChart, storageChart;
let history = [];
const MAX_HISTORY = 30;
let currentUploadXHR = null;


function initCharts() {

    cpuChart = new Chart(document.getElementById('cpu-usage'), {
        type: 'line',
        data: { datasets: [{ label: 'CPU Usage', data: [] }] },
        options: { responsive: true, scales: { y: { min: 0, max: 100 } } }
    });

    memoryChart = new Chart(document.getElementById('memory-chart'), {
        type: 'bar',
        data: {
            labels: ['Free Heap', 'Min Free', 'PSRAM'],
            datasets: [{ label: 'Bytes', data: [] }]
        }
    });

    storageChart = new Chart(document.getElementById('storage-chart'), {
        type: 'doughnut',
        data: { labels: [], datasets: [{ data: [], backgroundColor: [] }] }
    });
}


function updateStats() {

    $.get('/api/stats', function(data) {

        // data= JSON removed

        $('#temp').text(data.cpu.temperature.toFixed(1));
        $('#ssid').text(data.network.ssid);
        $('#ip').text(data.network.ip);
        $('#rssi').text(data.network.rssi);

        updateCPUChart(data.cpu.usage);

        updateMemoryChart(
            { free: data.memory.free, min_free: data.memory.min },
            { free: data.memory.psram_free }
        );

        updateStorageChart({
            littlefs: { used: data.storage.littlefs_used },
            spiffs: { used: data.storage.spiffs_used },
            sd: data.storage.sd_available ? { used: 1 } : null
        });
    });
}

function updateCPUChart(usage) {

    history.push(usage);

    if(history.length > MAX_HISTORY) history.shift();

    cpuChart.data.labels = Array.from({length: history.length}, (_,i) => i);
    cpuChart.data.datasets[0].data = history;
    cpuChart.update();
}

function updateMemoryChart(heap, psram) {
    memoryChart.data.datasets[0].data = [
        heap.free,
        heap.min_free,
        psram ? psram.free : 0
    ];
    memoryChart.update();
}



function updateStorageChart(fs) {
    let labels = [];
    let data = [];
    let colors = [];

    if(fs.littlefs) {
        labels.push('LittleFS');
        data.push(fs.littlefs.used);
        colors.push('#4CAF50');
    }

    if(fs.spiffs) {
        labels.push('SPIFFS');
        data.push(fs.spiffs.used);
        colors.push('#2196F3');
    }

    if(fs.sd) {
        labels.push('SD Card');
        data.push(fs.sd.used);
        colors.push('#FF9800');
    }

    storageChart.data.labels = labels;
    storageChart.data.datasets[0].data = data;
    storageChart.data.datasets[0].backgroundColor = colors;
    storageChart.update();
}


// FILE EXPLORER
function refreshFiles() {

    const fsType = $('#fs-select').val();


    $.get(`/api/files?fs=${fsType}`, function(files) {

        const table = $('#files').empty();

        files.forEach(file => {
            table.append(`
            <tr>
            <td>${file.name}</td>
            <td>${formatBytes(file.size)}</td>
            <td>
            <button class="btn-secondary" onclick="downloadFile('${fsType}', '${file.name}')">Download</button>
            <button class="btn-secondary" onclick="deleteFile('${fsType}', '${file.name}')">Delete</button>
            </td>
            </tr>
            `);
        });
    });
}

function downloadFile(fs, name) {
    window.open(`/api/files/download?fs=${fs}&name=${name}`, '_blank');
}


function deleteFile(fs, name) {
    $.ajax({
        url: `/api/files?fs=${fs}&name=${name}`,
        type: 'DELETE',
        success: refreshFiles
    });
}

// Toda la morralla delUPLOAD, falta ajustar API
function createUploadModal() {
    const modalHTML = `
    <div id="upload-modal" class="upload-modal">
        <div class="upload-modal-content">
            <div class="upload-header">
                <h3>Uploading File</h3>
                <button id="cancel-upload" class="cancel-btn">&times;</button>
            </div>

            <div class="upload-body">
                <div class="upload-info">
                    <div class="file-name" id="upload-filename">filename.txt</div>
                        <div class="upload-stats">
                            <div class="stat">
                                <span class="label">Progress:</span>
                                <span class="value" id="upload-percentage">0%</span>
                            </div>
                            <div class="stat">
                                <span class="label">Speed:</span>
                                <span class="value" id="upload-speed">0 B/s</span>
                            </div>
                            <div class="stat">
                                <span class="label">Time remaining:</span>
                                <span class="value" id="upload-eta">Calculating...</span>
                            </div>
                            <div class="stat">
                                <span class="label">Transferred:</span>
                                <span class="value" id="upload-transferred">0 B / 0 B</span>
                            </div>
                        </div>
                    </div>
                    <div class="progress-container">
                        <div class="progress-bar">
                            <div class="progress" id="upload-progress-bar"></div>
                        </div>
                    </div>
                    <div class="upload-status" id="upload-status">
                        <div class="loading-dots">
                            <span></span><span></span><span></span>
                        </div>
                    <span>Uploading file...</span>
                    </div>
                </div>
            </div>
        </div>
    `;

    $('body').append(modalHTML);


    $('#cancel-upload').click(function() {

        cancelUpload();
    });
}


function showUploadModal(filename) {

    if (!$('#upload-modal').length) {
        createUploadModal();
    }

    $('#upload-filename').text(filename);
    $('#upload-percentage').text('0%');
    $('#upload-speed').text('0 B/s');
    $('#upload-eta').text('Calculating...');
    $('#upload-transferred').text('0 B / 0 B');
    $('#upload-progress-bar').css('width', '0%');
    $('#upload-status').html(`
    <div class="loading-dots">
        <span></span><span></span><span></span>
    </div>
    <span>Uploading file...</span>
    `);

    $('#upload-modal').fadeIn(300);
}

function hideUploadModal() {
    $('#upload-modal').fadeOut(300);
}

function updateUploadProgress(loaded, total, speed, eta) {
    const percentage = Math.round((loaded / total) * 100);

    $('#upload-percentage').text(percentage + '%');
    $('#upload-speed').text(formatBytes(speed) + '/s');
    $('#upload-eta').text(eta);
    $('#upload-transferred').text(formatBytes(loaded) + ' / ' + formatBytes(total));
    $('#upload-progress-bar').css('width', percentage + '%');
}

function showUploadSuccess() {
    $('#upload-status').html(`
    <div class="success-icon">✓</div>
    <span>Upload completed successfully!</span>
    `);

    setTimeout(() => {
        hideUploadModal();
        refreshFiles();
    }, 2000);
}

function showUploadError(error) {
    let errorMessage = 'Upload failed';

    if (error.status === 0) {
        errorMessage = 'Upload cancelled by user';
    } else if (error.status === 413) {
        errorMessage = 'File too large';
    } else if (error.status === 507) {
        errorMessage = 'Insufficient storage space';
    } else if (error.status >= 400 && error.status < 500) {
        errorMessage = 'Client error: ' + (error.responseJSON?.error || error.statusText);
    } else if (error.status >= 500) {
        errorMessage = 'Server error: ' + (error.responseJSON?.error || error.statusText);
    } else if (error.statusText === 'abort') {
        errorMessage = 'Upload cancelled';
    } else {
        errorMessage = 'Network error: ' + error.statusText;
    }

    $('#upload-status').html(`
    <div class="error-icon">✗</div>
    <span>${errorMessage}</span>
    `);

    setTimeout(() => {
        hideUploadModal();
    }, 3000);
}

function cancelUpload() {
    if (currentUploadXHR) {
        currentUploadXHR.abort();
        currentUploadXHR = null;
    }
    hideUploadModal();
}


// PROGRESS MODAL
$('#upload-form').submit(function(e) {
    e.preventDefault();

    const fsType = $('#fs-select').val();
    const file = $('#file-input')[0].files[0];

    if (!file) {
        alert('Please select a file first');
        return;
    }

    const formData = new FormData();
    formData.append('file', file);

    showUploadModal(file.name);



    let startTime = Date.now();
    let lastLoaded = 0;
    let lastTime = startTime;


    currentUploadXHR = $.ajax({
        url: `/api/files/upload?fs=${fsType}`,
        type: 'POST',
        data: formData,
        processData: false,
        contentType: false,
        xhr: function() {
            const xhr = new window.XMLHttpRequest();

            xhr.upload.addEventListener('progress', function(e) {
                if (e.lengthComputable) {
                    const currentTime = Date.now();
                    const timeDiff = (currentTime - lastTime) / 1000; // segundos
                    const loadedDiff = e.loaded - lastLoaded;


                    // fileSize / time pending
                    let speed = 0;
                    if (timeDiff > 0) {
                        speed = loadedDiff / timeDiff;
                    }



                    // filesize / bytes
                    let eta = 'Calculating...';

                    if (speed > 0) {
                        const remainingBytes = e.total - e.loaded;
                        const remainingSeconds = remainingBytes / speed;

                        if (remainingSeconds < 60) {
                            eta = Math.round(remainingSeconds) + 's';

                        } else if (remainingSeconds < 3600) {
                            eta = Math.round(remainingSeconds / 60) + 'm ' + Math.round(remainingSeconds % 60) + 's';

                        } else {
                            const hours = Math.floor(remainingSeconds / 3600);
                            const minutes = Math.round((remainingSeconds % 3600) / 60);
                            eta = hours + 'h ' + minutes + 'm';
                        }
                    }

                    updateUploadProgress(e.loaded, e.total, speed, eta);

                    lastLoaded = e.loaded;
                    lastTime = currentTime;
                }
            }, false);

            return xhr;
        },
        success: function(response) {
            currentUploadXHR = null;
            showUploadSuccess();
        },
        error: function(xhr, status, error) {
            currentUploadXHR = null;
            showUploadError(xhr);
        }
    });
});


function formatBytes(bytes) {
    if (bytes === 0) return '0 B';
    const k = 1024, sizes = ['B', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}


// DOM start
$(document).ready(function() {
    initCharts();
    updateStats();
    setInterval(updateStats, 5000);
    refreshFiles();
});
